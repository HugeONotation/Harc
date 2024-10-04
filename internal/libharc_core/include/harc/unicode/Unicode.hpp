#ifndef HARC_UNICODE_HPP
#define HARC_UNICODE_HPP

#include <array>
#include <algorithm>

#include "Unicode_tables.hpp"

namespace harc {

    Codepoint_category categorized_codepoint(std::uint32_t x) {
        // Use lookup table for ASCII codepoints
        if (x < 128) {
            return codepoint_categories7[x];
        }

        // Search for program
        for (std::uint16_t codepoint : non_ascii_whitespace_codepoints) {
            if (codepoint == x) {
                return Codepoint_category::OTHER_WHITESPACE;
            }
        }

        // Search through lookup tables for codepoints that have up to 16 bits
        if (x <= UINT16_MAX) {
            auto a = std::begin(identifier_codepoint_range_firsts_16);
            auto b = std::end(identifier_codepoint_range_firsts_16);
            auto it = std::lower_bound(a, b, std::uint16_t(x));

            auto range_begin = *it;
            auto range_end = identifier_codepoint_range_sizes_16[it - a];

            if (range_begin <= x && x < range_end) {
                std::uint32_t index = (it - a);
                std::uint32_t uint_index = index / 32;
                std::uint32_t bit_index  = index % 32;
                bool bit = 1 & (identifier_codepoint_categories_16[uint_index] >> bit_index);
                return bit ? Codepoint_category::IDENTIFIER_START : Codepoint_category::IDENTIFIER_CONTINUE;
            }

            return Codepoint_category::UNRECOGNIZED;
        }

        // Search through lookup tables for codepoints that have up to 21 bits
        {
            auto a = std::begin(identifier_codepoint_range_firsts_21);
            auto b = std::end(identifier_codepoint_range_firsts_21);
            auto it = std::lower_bound(a, b, x);

            auto range_begin = *it;
            auto range_end = identifier_codepoint_range_sizes_21[it - a];

            if (range_begin <= x && x < range_end) {
                std::uint32_t index = (it - a);
                std::uint32_t uint_index = index / 32;
                std::uint32_t bit_index  = index % 32;
                bool bit = 1 & (identifier_codepoint_categories_21[uint_index] >> bit_index);
                return bit ? Codepoint_category::IDENTIFIER_START : Codepoint_category::IDENTIFIER_CONTINUE;
            }

            return Codepoint_category::UNRECOGNIZED;
        }
    }

}

#endif //HARC_UNICODE_HPP
