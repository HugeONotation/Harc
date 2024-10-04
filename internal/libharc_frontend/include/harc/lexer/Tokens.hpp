#ifndef HARC_TOKENS_HPP
#define HARC_TOKENS_HPP

#include <cstdint>
#include <string_view>

namespace harc {

    ///
    /// An enum class for representing a token type.
    ///
    /// The tokens are presented here in an order which is convenient, but
    /// which does not correspond to their underling representation. Their
    /// backing values are chosen from the index in the operator_table
    /// array found within the harc::lex::identify_token function.
    ///
    enum class Token_type : std::uint8_t {
        NULL_TOKEN = 0x0,

        // Brackets
        LEFT_PARENTHESIS       = 0x0b,
        RIGHT_PARENTHESIS      = 0x0c,
        LEFT_SQUARE_BRACKET    = 0x35,
        RIGHT_SQUARE_BRACKET   = 0x36,
        LEFT_CURLY_BRACKET     = 0x3c,
        RIGHT_CURLY_BRACKET    = 0x43,
        LEFT_TEMPLATE_BRACKET  = 0x2b,
        RIGHT_TEMPLATE_BRACKET = 0x33,
        LEFT_COMMENT_BRACKET   = 0x1f,
        RIGHT_COMMENT_BRACKET  = 0x0d,

        // Operator tokens

        PREFIX_PLUS      = 0x14,
        PREFIX_MINUS     = 0x1b,
        PREFIX_INCREMENT = 0x10,
        SUFFIX_INCREMENT = 0x11,
        PREFIX_DECREMENT = 0x16,
        SUFFIX_DECREMENT = 0x17,
        SPACED_PERCENT   = 0x05,
        SPACED_ASTERISK  = 0x0f,
        SPACED_PLUS      = 0x13,
        SPACED_MINUS     = 0x1a,
        SPACED_SLASH     = 0x21,

        PREFIX_EXCLAMATION = 0x02,
        SPACED_LOGICAL_AND = 0x07,
        SPACED_LOGICAL_OR  = 0x40,
        SPACED_LOGICAL_XOR = 0x39,

        PREFIX_TILDE                  = 0x44,
        SPACED_AMPERSAND              = 0x09,
        SPACED_CARET                  = 0x3a,
        SPACED_PIPE                   = 0x42,
        SPACED_MODULAR_LEFT_SHIFT     = 0x27,
        SPACED_MODULAR_RIGHT_SHIFT    = 0x31,
        SPACED_SATURATING_LEFT_SHIFT  = 0x29,
        SPACED_SATURATING_RIGHT_SHIFT = 0x41,

        SPACED_PLUS_EQUALS  = 0x12,
        SPACED_MINUS_EQUALS = 0x18,
        SPACED_TIMES_EQUALS = 0x0e,
        SPACED_DIV_EQUALS   = 0x20,
        SPACED_REM_EQUALS   = 0x04,

        SPACED_BITWISE_AND_EQUALS = 0x08,
        SPACED_BITWISE_OR_EQUALS  = 0x3f,
        SPACED_BITWISE_XOR_EQUALS = 0x38,

        SPACED_LOGICAL_AND_EQUALS = 0x06,
        SPACED_LOGICAL_OR_EQUALS  = 0x3e,
        SPACED_LOGICAL_XOR_EQUALS = 0x37,

        SPACED_MODULAR_LEFT_SHIFT_EQUALS     = 0x26,
        SPACED_MODULAR_RIGHT_SHIFT_EQUALS    = 0x2f,
        SPACED_SATURATING_LEFT_SHIFT_EQUALS  = 0x25,
        SPACED_SATURATING_RIGHT_SHIFT_EQuALS = 0x3d,

        JOINED_DOUBLE_COLON = 0x22,
        JOINED_SINGLE_ARROW = 0x19,
        SPACED_DOUBLE_ARROW = 0x2d,

        SPACED_COMPARE_EQ = 0x2c,
        SPACED_COMPARE_NE = 0x01,
        SPACED_COMPARE_LT = 0x2a,
        SPACED_COMPARE_GT = 0x32,
        SPACED_COMPARE_LE = 0x28,
        SPACED_COMPARE_GE = 0x30,

        SPACED_EQUALS        = 0x2e,
        JOINED_DOUBLE_PERIOD = 0x1d,
        SUFFIX_TRIPLE_PERIOD = 0x1c,

        // Misc. Tokens
        SPACED_PERIOD = 0x1e,
        COMMA         = 0x15,
        AT_SIGN       = 0x34,
        HASH          = 0x03,
        COLON         = 0x23,
        SEMICOLON     = 0x24,
        SUFFIX_AMPERSAND = 0x0a,
        SUFFIX_CARET     = 0x3b,

        // Textual tokens
        TEXT                 = 250,
        NUMERIC_LITERAL      = 251,
        CODEPOINT_LITERAL    = 252,
        STRING_LITERAL       = 253,
        SINGLE_LINE_COMMENT  = 254,
        DOC_TEXT             = 255
    };

    ///
    /// \param type Arbitrary Token_type object
    /// \return True if the token type is one of NUMERIC_LITERAL,
    /// STRING_LITERAL, CHAR_LITERAL, TEXT, or DOC_TEXT
    [[nodiscard]]
    bool is_textual(Token_type type);

    /*
    ///
    /// \param type Arbitrary Token_type object
    /// \return True if the token corresponds to a unary operator
    [[nodiscard]]
    bool is_unary_operator(Token_type type);
    */

    /*
    ///
    /// \param type Arbitrary Token_type object
    /// \return True if the token corresponds to a binary operator
    [[nodiscard]]
    bool is_binary_operator(Token_type type);
    */

    ///
    /// \param type Arbitrary Token_type object
    /// \return True if the token type has a corresponding right token.
    [[nodiscard]]
    bool is_left_token(Token_type type);

    ///
    /// \param type Arbitrary Token_type object
    /// \return True if the token type has a corresponding left token.
    [[nodiscard]]
    bool is_right_token(Token_type type);

    ///
    /// The number of unique code points which constitute a codepoint, not
    /// including whitespace.
    ///
    /// For NULL_TOKEN, returns 0.
    /// For textual tokens, returns SIZE_MAX.
    ///
    [[nodiscard]]
    std::size_t token_width(Token_type type);

    ///
    /// \param token_type Token_type enum value
    /// \return View over string representation of token type. View remains
    /// valid throughout life of program.
    [[nodiscard]]
    std::string_view to_string(Token_type token_type);

}

#endif //HARC_TOKENS_HPP
