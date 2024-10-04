#include <cstdint>

#include <algorithm>

#include <harc/cuda_common/Codepoint_manipulation_tables.cuh>
#include <harc/cuda_common/Algorithms.cuh>

namespace harc::cuda {

    ///
    /// \param codepoint Unicode codepoint to query width of
    /// \return Number of bytes required to encode the specified codepoint in utf8
    __device__
    std::uint32_t codepoint_utf8_encoding_width(std::uint32_t codepoint) {
        std::uint32_t ret = 1;

        ret += codepoint >= (1 << 7);
        ret += codepoint >= (1 << 11);
        ret += codepoint >= (1 << 16);

        return ret;
    }

    [[nodiscard]]
    __device__
    Codepoint_category lookup_identifier_codepoint_16(std::uint16_t c) {
        const std::uint16_t* range_begin = lower_bound(identifier_codepoint_range_firsts_16, c);
        auto index = range_begin - identifier_codepoint_range_firsts_16;
        const std::uint16_t* range_length = identifier_codepoint_range_sizes_16 + index;

        // Initial values prevent a < c && c < b from ever being true if a, b
        // not assigned to
        std::uint16_t a = 1;
        std::uint16_t b = 0;

        if (range_begin != end(identifier_codepoint_range_firsts_16)) {
            a = *range_begin;
            b = a + *range_length;
        }

        // The following section assumes this holds true. Assumption can be
        // relaxed by conditionally zeroing out index in an else statement to
        // the above if statement.
        static_assert(extent(identifier_codepoint_range_firsts_16) / 32 < extent(identifier_codepoint_categories_16));

        // Extract the bit encoding the category for c
        std::uint32_t index_whole = index / 32;
        std::uint32_t index_part  = index % 32;

        auto field = identifier_codepoint_categories_16[index_whole];
        bool category_bit = (field >> index_part) & 0x1;

        auto ret = Codepoint_category::UNRECOGNIZED;
        if (a < c && c < b) {
            if (category_bit) {
                ret = Codepoint_category::IDENTIFIER_START;
            } else {
                ret = Codepoint_category::IDENTIFIER_CONTINUE;
            }
        }

        return ret;
    }

    [[nodiscard]]
    __device__
    Codepoint_category lookup_identifier_codepoint_21(std::uint32_t c) {
        const std::uint32_t* range_begin = lower_bound(identifier_codepoint_range_firsts_21, c);
        auto index = range_begin - identifier_codepoint_range_firsts_21;
        const std::uint16_t* range_length = identifier_codepoint_range_sizes_21 + index;

        // Initial values prevent a < c && c < b from ever being true if a, b
        // not assigned to
        std::uint16_t a = 1;
        std::uint16_t b = 0;

        if (range_begin != end(identifier_codepoint_range_firsts_21)) {
            a = *range_begin;
            b = a + *range_length;
        }

        // The following section assumes this holds true. Assumption can be
        // relaxed by conditionally zeroing out index in an else statement to
        // the above if statement.
        static_assert(extent(identifier_codepoint_range_firsts_21) / 32 < extent(identifier_codepoint_categories_21));

        // Extract the bit encoding the category for c
        std::uint32_t index_whole = index / 32;
        std::uint32_t index_part  = index % 32;

        auto field = identifier_codepoint_categories_16[index_whole];
        bool category_bit = (field >> index_part) & 0x1;

        auto ret = Codepoint_category::UNRECOGNIZED;
        if (a < c && c < b) {
            if (category_bit) {
                ret = Codepoint_category::IDENTIFIER_START;
            } else {
                ret = Codepoint_category::IDENTIFIER_CONTINUE;
            }
        }

        return ret;
    }

    ///
    /// \param c ASCII character.
    /// \return Category to which the codepoint belongs
    __device__
    Codepoint_category categorize_codepoint(std::uint8_t c) {
        return codepoint_categories7[umin(std::uint8_t(127), c)];
    }

    ///
    /// \param c Unicode codepoint
    /// \return Category to which the codepoint belongs
    __device__
    Codepoint_category categorized_codepoint(std::uint32_t c) {
        Codepoint_category ret = codepoint_categories7[umin(std::uint8_t(127), c)];

        constexpr std::uint32_t full_mask = 0xffffffff;
        bool is_warp_pure_ascii = __all_sync(full_mask, c < (1 << 7));
        if (is_warp_pure_ascii) {
            return ret;
        }


        // Check if the codepoint is whitespace
        if (lower_bound_find(non_ascii_whitespace_codepoints, std::uint16_t(umax(c, (1 << 16) - 1)))) {
            ret = Codepoint_category::OTHER_WHITESPACE;
        }

        bool is_warp_limited_to_16_bit = __all_sync(full_mask, c < (1 << 16));
        if (is_warp_limited_to_16_bit) {
            // Check if the codepoint is a recognized identifier codepoint
            Codepoint_category lookup_category = lookup_identifier_codepoint_16(c);
            if (lookup_category != Codepoint_category::UNRECOGNIZED) {
                ret = lookup_category;
            }

            return ret;
        }

        // Check if the codepoint is a recognized identifier codepoint
        Codepoint_category lookup_category = lookup_identifier_codepoint_21(c);
        if (lookup_category != Codepoint_category::UNRECOGNIZED) {
            ret = lookup_category;
        }

        return ret;
    }

}
