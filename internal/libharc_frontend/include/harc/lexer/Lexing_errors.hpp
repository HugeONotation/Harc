#ifndef HARMONIA_LEXING_ERRORS_HPP
#define HARMONIA_LEXING_ERRORS_HPP

#include <cstdint>

namespace harc::lex {

    enum class Error_code : std::uint16_t {
        NO_ERROR,

        // ASCII errors
        INVALID_ASCII_VALUE_GREATER_THAN_127,
        UNRECOGNIZED_CHARACTER,

        // utf8 validation errors
        INVALID_UTF8,
        INVALID_UTF8_NON_SHORTEST_FORM,
        INVALID_UTF8_HIGH_TWO_BITS_NOT_SET_IN_CONTINUATION_BYTES,
        INVALID_UTF8_EXCESSIVE_CONTINUATION_BYTES,
        INVALID_UTF8_CODEPOINT_IS_SURROGATE_VALUE,
        INVALID_UTF8_CODEPOINT_GREATER_THAN_U10FFFF,
        UNRECOGNIZED_CODEPOINT,

        // General tokenization errors
        UNRECOGNIZED_TOKEN_TYPE,

        // Token balancing errors
        UNMATCHED_DOUBLE_QUOTES,
        UNMATCHED_SINGLE_QUOTE,
        UNMATCHED_PARENTHESIS,
        UNMATCHED_CURLY_BRACKET,
        UNMATCHED_SQUARE_BRACKET,
        UNMATCHED_TEMPLATE_BRACKETS,

        // Internal errors
        UNHANDLED_SWITCH_BRANCH
    };

    ///
    /// Struct representing an error that occurred during tokenization.
    ///
    struct Error {

        ///
        /// Kind of error encountered
        ///
        lex::Error_code error_code{};

        ///
        /// Beginning of half-open range where error occurred
        ///
        std::uint32_t error_begin = 0;

        ///
        /// End of half-open range where error occurred
        ///
        std::uint32_t error_end = 0;
    };



}

#endif //HARMONIA_LEXING_ERRORS_HPP
