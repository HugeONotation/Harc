#include <harc/prepass/Prepass.hpp>

#include <cstring>

namespace harc::prepass {

    std::uint64_t fmix64(std::uint64_t k) {
        // Helper function used in computation of MurmurHash3_x86_128
        k ^= k >> 33;
        k *= 0xff51afd7ed558ccdull;
        k ^= k >> 33;
        k *= 0xc4ceb9fe1a85ec53ull;
        k ^= k >> 33;

        return k;
    }

    Prepass_results prepass(const Translation_unit& unit) {
        std::uint32_t codepoint_count = unit.source.size();

        // MurmurHash3_x86_128 state
        std::uint64_t h1 = 0x0123456789abcdef;
        std::uint64_t h2 = 0xfdecba9876543210;

        constexpr std::uint32_t block_size = 16;

        constexpr std::uint64_t c1 = 0x87c37b91114253d5ull;
        constexpr std::uint64_t c2 = 0x4cf5ad432745937full;

        std::uint64_t whole_blocks = unit.source.size() / block_size;
        std::uint64_t block_remainders = unit.source.size() % block_size;

        // Process whole blocks
        std::uint64_t i = 0;
        for (; i < whole_blocks * block_size; i += block_size) {
            // Load 128-bit block
            std::uint64_t block0;
            std::memcpy(&block0, unit.source.data() + i, sizeof(std::uint64_t));

            std::uint64_t block1;
            std::memcpy(&block1, unit.source.data() + i + sizeof(std::uint64_t), sizeof(std::uint64_t));

            // Codepoint counting logic
            codepoint_count -= std::popcount(((~block0 << 1) & block0) & 0x8080808080808080ull);
            codepoint_count -= std::popcount(((~block1 << 1) & block1) & 0x8080808080808080ull);

            // MurmurHash3_x86_128 logic
            block0 *= c1;
            block0 = std::rotl(block0, 31);
            block0 *= c2;
            h1 ^= block0;

            h1 = std::rotl(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;

            block1 *= c2;
            block1 = std::rotl(block1, 33);
            block1 *= c1;
            h2 ^= block1;

            h2 = std::rotl(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
        }

        // Process final partial block
        if (block_remainders != 0) {
            // Load 128-bit block
            std::uint64_t block0 = 0;
            std::memcpy(
                &block0,
                unit.source.data() + i,
                std::min(block_remainders, sizeof(std::uint64_t))
            );

            std::uint64_t block1 = 0;
            std::memcpy(
                &block1,
                unit.source.data() + i + block_size /  2,
                block_remainders - std::min(block_remainders, sizeof(std::uint64_t))
            );

            // Codepoint counting logic
            codepoint_count -= std::popcount(((~block0 << 1) & block0) & 0x8080808080808080ull);
            codepoint_count -= std::popcount(((~block1 << 1) & block1) & 0x8080808080808080ull);

            // MurmurHash3_x86_128 logic
            block0 *= c1;
            block0  = std::rotl(block0, 31);
            block0 *= c2;
            h1 ^= block0;

            block1 *= c2;
            block1  = std::rotl(block1, 33);
            block1 = c1;
            h2 ^= block1;
        }

        // MurmurHash3_x86_128 finalization logic
        h1 ^= unit.source.size();
        h2 ^= unit.source.size();

        h1 += h2;
        h2 += h1;

        h1 = fmix64(h1);
        h2 = fmix64(h2);

        h1 += h2;
        h2 += h1;

        //Populate output variable
        Prepass_results results;
        results.is_ascii = codepoint_count == unit.source.size();
        results.codepoint_count = codepoint_count;
        results.hash[0] = h1;
        results.hash[1] = h2;

        return results;
    }

}
