#include <harc/Caching.hpp>
#include <harc/Version.hpp>

#include <aul/Math.hpp>

#include <cstring>

namespace harc {

    const Version serialization_version{0, 0, 0, Release_type::ALPHA};

    std::vector<std::byte> assemble_serialization(
        std::span<const Cache_entry_info> infos
    ) {
        constexpr std::size_t format_header_size = 16;
        constexpr std::size_t index_entry_overhead = 24;
        constexpr std::size_t entry_header_size = 32;

        // Compute size of serialized data
        std::size_t serialization_size = 0;
        serialization_size += format_header_size;

        for (const auto& info : infos) {
            serialization_size += index_entry_overhead;

            serialization_size += sizeof(std::uint32_t);
            serialization_size += 4 * aul::divide_ceil(info.path.size(), std::size_t(4));

            serialization_size += entry_header_size;
            serialization_size += info.tokenization_serialization.size();
            serialization_size += info.parsing_serialization.size();
            serialization_size += info.ir_serialization.size();
            serialization_size += info.machine_code_serialization.size();
        }

        std::vector<std::byte> ret;
        ret.resize(serialization_size);

        // Write data to buffer:
        std::byte* output_ptr = ret.data();

        // Write region 0
        // Write serialization header
        const char* magic_bytes = "HarCache";
        std::memcpy(output_ptr, magic_bytes, 8);
        output_ptr += 8;

        const std::uint32_t version_bytes = to_int(serialization_version);
        std::memcpy(output_ptr, &version_bytes, sizeof(std::uint32_t));
        output_ptr += sizeof(std::uint32_t);

        std::memset(output_ptr, 0x00, 4);
        output_ptr += 4;

        // Assemble serialization index header
        std::uint64_t entry_count = infos.size();
        std::memcpy(&output_ptr, &entry_count, sizeof(std::uint64_t));
        output_ptr += sizeof(std::uint64_t);

        // Write region 1
        // Assemble entries
        std::uint64_t region2_offset = 0;
        for (auto& info : infos) {
            // Write offset
            std::memcpy(output_ptr, &region2_offset, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            // Write  timestamp;
            std::memcpy(output_ptr, &info.timestamp, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            // Write file path
            std::uint32_t path_length = info.path.size();
            std::memcpy(&output_ptr, &path_length, sizeof(std::uint32_t));
            output_ptr += sizeof(std::uint32_t);

            std::memcpy(output_ptr, info.path.data(), info.path.size());
            output_ptr += info.path.size();

            std::uint32_t padding_byte_count = (8 - (sizeof(std::uint32_t) + info.path.size()) % 8 ) % 8;
            std::memset(output_ptr, 0x00, padding_byte_count);
            output_ptr += padding_byte_count;

            // Adjust region offset
            std::uint64_t entry_size = 0;
            entry_size += entry_header_size;
            entry_size += info.tokenization_serialization.size();
            entry_size += info.parsing_serialization.size();
            entry_size += info.ir_serialization.size();
            entry_size += info.machine_code_serialization.size();
            region2_offset += entry_size;
        }

        // Region 2
        // Write cache entries
        for (auto info : infos) {
            // Write entry header
            std::uint64_t lexer_offset = 0;
            std::memcpy(output_ptr, &lexer_offset, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            std::uint64_t parse_offset = info.tokenization_serialization.size();
            std::memcpy(output_ptr, &parse_offset, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            std::uint64_t ir_offset = parse_offset += info.parsing_serialization.size();
            std::memcpy(output_ptr, &ir_offset, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            std::uint64_t machine_code_offset = ir_offset + info.ir_serialization.size();
            std::memcpy(output_ptr, &machine_code_offset, sizeof(std::uint64_t));
            output_ptr += sizeof(std::uint64_t);

            // Write sub-entries
            std::memcpy(output_ptr, info.tokenization_serialization.data(), info.tokenization_serialization.size());
            output_ptr += info.tokenization_serialization.size();

            std::memcpy(output_ptr, info.parsing_serialization.data(), info.parsing_serialization.size());
            output_ptr += info.parsing_serialization.size();

            std::memcpy(output_ptr, info.ir_serialization.data(), info.ir_serialization.size());
            output_ptr += info.ir_serialization.size();

            std::memcpy(output_ptr, info.machine_code_serialization.data(), info.machine_code_serialization.size());
            output_ptr += info.machine_code_serialization.size();
        }

        return ret;
    }

    std::vector<Cache_entry_info> parse_serialization(std::span<const std::byte> bytes) {
        //TODO: Perform proper error handling

        if (bytes.size() < 24) {
            return {};
            // Input is too small to be a valid serialization
            // 16 bytes for serialization header and 8 bytes for index table
            // entry count required
        }

        constexpr std::array<std::byte, 8> magic_bytes {
            std::byte('H'), std::byte('a'), std::byte('r'), std::byte('C'),
            std::byte('a'), std::byte('c'), std::byte('h'), std::byte('e')
        };

        bool contains_magic_bytes = std::equal(
            magic_bytes.begin(),
            magic_bytes.end(),
            bytes.begin()
        );

        if (!contains_magic_bytes) {
            return {}; // Serialization header not found
        }

        std::uint64_t offset = sizeof(magic_bytes);

        // Read version
        std::uint32_t version;
        std::memcpy(&version, bytes.data() + offset, sizeof(std::uint32_t));
        offset += sizeof(std::uint32_t);

        if (to_int(serialization_version) != version) {
            return {};
            // Version read from header did not match expected value
        }

        // Jump over padding bytes in serialization header
        offset += sizeof(std::uint32_t);

        // Read index entry count
        std::uint64_t index_entry_count = 0;
        std::memcpy(&index_entry_count, bytes.data() + offset, sizeof(std::uint64_t));
        offset += sizeof(std::uint64_t);

        // Return early if serialization does not contain any entries
        if (index_entry_count == 0) {
            return {};
        }

        // Read index entries
        std::vector<Cache_entry_info> infos;
        infos.resize(index_entry_count);

        std::vector<std::uint64_t> cache_entry_offsets{};
        cache_entry_offsets.resize(infos.size());

        for (std::uint64_t i = 0; i < index_entry_count; ++i) {
            //Ensure that serialization contains enough data
            if (offset + 2 * sizeof(std::uint64_t) + sizeof(std::uint32_t)) {
                return {};
                // Not enough bytes in the serialization to read index entry
            }

            // Read cache entry offset
            std::uint64_t cache_entry_offset = 0;
            std::memcpy(&cache_entry_offset, bytes.data() + offset, sizeof(std::uint64_t));
            offset += 64;
            cache_entry_offsets[i] = cache_entry_offset;

            // Read timestamp
            std::uint64_t timestamp = 0;
            std::memcpy(&timestamp, bytes.data() + offset, sizeof(std::uint64_t));
            offset += 64;

            infos[i].timestamp = timestamp;

            // Read path size
            std::uint32_t path_size = 0;

            // Ensure that enough bytes exist to be read from
            if (bytes.size() <= offset + path_size ) {
                return {};
                // Path was longer than amount of remaining bytes
            }

            // Read path
            std::string_view path{
                reinterpret_cast<const char*>(bytes.data() + offset),
                path_size
            };

            infos[i].path = path;
        }

        std::uint64_t region2_offset = offset;

        // Read entries
        for (std::uint64_t i = 0; i < index_entry_count; ++i) {
            //TODO: Complete implementation
            infos[i].tokenization_serialization = std::span<const std::byte>{

            };

            infos[i].parsing_serialization = std::span<const std::byte>{

            };

            infos[i].ir_serialization = std::span<const std::byte>{

            };

            infos[i].machine_code_serialization = std::span<const std::byte>{

            };
        }

        return infos;
    }

}
