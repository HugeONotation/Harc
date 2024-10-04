#ifndef HARC_CORE_CACHING_HPP
#define HARC_CORE_CACHING_HPP

#include <vector>
#include <span>
#include <cstdint>
#include <string_view>

namespace harc {

    struct Cache_entry_info {
        std::string_view path;
        std::uint64_t timestamp = 0;
        std::span<const std::byte> tokenization_serialization;
        std::span<const std::byte> parsing_serialization;
        std::span<const std::byte> ir_serialization;
        std::span<const std::byte> machine_code_serialization;
    };

    ///
    /// Takes the serializations for different compilation stages within Harc
    /// and then assembles them within the Harc cache serialization format.
    ///
    /// \param infos View over the infos to use to assembly the data
    /// \return The serialization bytes
    [[nodiscard]]
    std::vector<std::byte> assemble_serialization(
        std::span<const Cache_entry_info> infos
    );

    std::vector<Cache_entry_info> parse_serialization();

}

#endif //HARC_CORE_CACHING_HPP
