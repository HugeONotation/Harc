#ifndef HARC_CODEPOINT_MANIPULATION_TABLES_HPP
#define HARC_CODEPOINT_MANIPULATION_TABLES_HPP

#include <cstdint>
#include <type_traits>

namespace harc {

    ///
    /// Enum for categorizations of codepoints as is relevant for Harmonia's
    /// grammar.
    ///
    enum class Codepoint_category : std::uint8_t {
        UNRECOGNIZED = 0,

        IDENTIFIER_START = 1,
        IDENTIFIER_CONTINUE = 2,
        NEWLINE = 3,
        OTHER_WHITESPACE = 4,

        EXCLAMATION = '!',
        DOUBLE_QUOTES = '"',
        SINGLE_QUOTE = '\'',
        CARET = '^',
        AMPERSAND = '&',
        PIPE = '|',
        TILDE = '~',
        COLON = ':',
        SEMICOLON = ';',
        COMMA = ',',

        PLUS = '+',
        MINUS = '-',
        ASTERISK = '*',
        FORWARD_SLASH = '/',
        PERCENT_SIGN = '%',

        BACKSLASH = '\\',

        EQUAL_SIGN = '=',

        L_PAREN = '(',
        R_PAREN = ')',
        L_CURLY_BRACKET  = '{',
        R_CURLY_BRACKET  = '}',
        L_SQUARE_BRACKET = '[',
        R_SQUARE_BRACKET = ']',
        L_ANGLE_BRACKET  = '<',
        R_ANGLE_BRACKET  = '>',

        DOT = '.'
    };

    //=====================================================
    // 7-bit codepoint data
    //=====================================================

    ///
    /// Table containing categories to which first 128 Unicode points belong to
    ///
    inline constexpr Codepoint_category codepoint_categories7[128] {
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::OTHER_WHITESPACE,
        Codepoint_category::NEWLINE,
        Codepoint_category::OTHER_WHITESPACE,
        Codepoint_category::OTHER_WHITESPACE,
        Codepoint_category::OTHER_WHITESPACE,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::OTHER_WHITESPACE,
        Codepoint_category::EXCLAMATION,
        Codepoint_category::DOUBLE_QUOTES,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::PERCENT_SIGN,
        Codepoint_category::AMPERSAND,
        Codepoint_category::SINGLE_QUOTE,
        Codepoint_category::L_PAREN,
        Codepoint_category::R_PAREN,
        Codepoint_category::ASTERISK,
        Codepoint_category::PLUS,
        Codepoint_category::COMMA,
        Codepoint_category::MINUS,
        Codepoint_category::DOT,
        Codepoint_category::FORWARD_SLASH,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::COLON,
        Codepoint_category::SEMICOLON,
        Codepoint_category::L_ANGLE_BRACKET,
        Codepoint_category::EQUAL_SIGN,
        Codepoint_category::R_ANGLE_BRACKET,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::L_SQUARE_BRACKET,
        Codepoint_category::BACKSLASH,
        Codepoint_category::R_SQUARE_BRACKET,
        Codepoint_category::CARET,
        Codepoint_category::IDENTIFIER_CONTINUE,
        Codepoint_category::UNRECOGNIZED,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::IDENTIFIER_START,
        Codepoint_category::L_CURLY_BRACKET,
        Codepoint_category::PIPE,
        Codepoint_category::R_CURLY_BRACKET,
        Codepoint_category::TILDE,
        Codepoint_category::UNRECOGNIZED
    };

    //=====================================================
    // 8 through 16-bit codepoint data
    //=====================================================

    ///
    /// List of codepoints which may be used within identifiers, starting from
    /// 2^8, up to 2^16 - 1.
    ///
    inline constexpr std::uint16_t identifier_codepoint_range_firsts_16[] {
$CODEPOINT16_BEGINS
    };

    ///
    /// Array parallel to identifier_codepoint_range_first_16
    ///
    /// Contains the lengths of the ranges whose first elements are contained
    /// in identifier_codepoint_range_first_16.
    ///
    inline constexpr std::uint16_t identifier_codepoint_range_sizes_16[] {
$CODEPOINT16_ENDS
    };

    static_assert(
        std::is_same_v<decltype(identifier_codepoint_range_firsts_16), decltype(identifier_codepoint_range_sizes_16)>
    );

    /// Array containing categorizations of codepoint ranges listed in
    /// identifier_codepoint_range_firsts_16 and
    /// identifier_codepoint_range_sizes_16. These categorizations are encoded
    /// as individual bits. A 1 indicates IDENTIFIER_START, and a 0 indicates
    /// IDENTIFIER_CONTINUE
    inline constexpr std::uint32_t identifier_codepoint_categories_16[] {
$CODEPOINT16_CATEGORIES
    };

    //=====================================================
    // 8 through 21-bit codepoint data
    //=====================================================

    ///
    /// List of codepoints which may be used within identifiers, starting from
    /// 2^8, up to 2^21 - 1
    ///
    inline constexpr std::uint32_t identifier_codepoint_range_firsts_21[] {
$CODEPOINT21_BEGINS
    };

    ///
    /// Array parallel to identifier_codepoints_21.
    ///
    /// Contains the lengths of the ranges whose first elements are listed in
    /// identifier_codepoints_21
    ///
    inline constexpr std::uint16_t identifier_codepoint_range_sizes_21[] {
$CODEPOINT21_ENDS
    };

    static_assert(
        std::extent<decltype(identifier_codepoint_range_firsts_21)>::value == std::extent<decltype(identifier_codepoint_range_sizes_21)>::value
    );

    ///
    /// Array containing categorizations of codepoint ranges listed in
    /// identifier_codepoint_range_firsts_21 and
    /// identifier_codepoint_range_sizes_21. These categorizations are encoded
    /// as individual bits. A 1 indicates IDENTIFIER_START, and a 0 indicates
    /// IDENTIFIER_CONTINUE
    ///
    inline constexpr std::uint32_t identifier_codepoint_categories_21[] {
$CODEPOINT21_CATEGORIES
    };

    //=====================================================
    // Whitespace codepoint data
    //=====================================================

    ///
    /// List of codepoints which are larger than 2^8 and which are recognized as
    /// whitespace.
    ///
    inline constexpr std::uint16_t non_ascii_whitespace_codepoints[] {
$NON_ASCII_WHITESPACE_ARRAY
    };

}

#endif //HARC_CODEPOINT_MANIPULATION_TABLES_HPP
