#include <harc/lexer/Tokens.hpp>

#include <cstdlib>
#include <array>

namespace harc {

    bool is_textual(Token_type type) {
        constexpr std::array<Token_type, 6> textual_tokens{
            Token_type::TEXT,
            Token_type::NUMERIC_LITERAL,
            Token_type::CODEPOINT_LITERAL,
            Token_type::STRING_LITERAL,
            Token_type::SINGLE_LINE_COMMENT,
            Token_type::SINGLE_LINE_COMMENT
        };

        bool is_textual = false;
        for (auto t : textual_tokens) {
            is_textual |= (t == type);
        }

        return is_textual;
    }

    /*
    bool is_unary_operator(Token_type type) {
        switch (type) {
            case Token_type::EXCLAMATION:
            case Token_type::AMPERSAND:
            case Token_type::SQUIGGLE:
            case Token_type::PLUS:
            case Token_type::MINUS:
            case Token_type::ASTERISK:
            case Token_type::INCREMENT:
            case Token_type::DECREMENT:
                return true;
            default:
                return false;
        }
    }
    */

    /*
    bool is_binary_operator(Token_type type) {

    }
    */

    bool is_left_token(Token_type type) {
        switch (type) {
            case Token_type::LEFT_PARENTHESIS:
            case Token_type::LEFT_SQUARE_BRACKET:
            case Token_type::LEFT_CURLY_BRACKET:
            case Token_type::LEFT_TEMPLATE_BRACKET:
            case Token_type::LEFT_COMMENT_BRACKET:
                return true;

            default:
                return false;
        }
    }

    bool is_right_token(Token_type type) {
        switch (type) {
            case Token_type::RIGHT_PARENTHESIS:
            case Token_type::RIGHT_SQUARE_BRACKET:
            case Token_type::RIGHT_CURLY_BRACKET:
            case Token_type::RIGHT_TEMPLATE_BRACKET:
            case Token_type::RIGHT_COMMENT_BRACKET:
                return true;

            default:
                return false;
        }
    }

    Token_type matching_token_type(Token_type type) {
        switch (type) {
            case Token_type::LEFT_PARENTHESIS:  return Token_type::RIGHT_PARENTHESIS;
            case Token_type::RIGHT_PARENTHESIS: return Token_type::LEFT_PARENTHESIS;

            case Token_type::LEFT_SQUARE_BRACKET:  return Token_type::RIGHT_SQUARE_BRACKET;
            case Token_type::RIGHT_SQUARE_BRACKET: return Token_type::LEFT_SQUARE_BRACKET;

            case Token_type::LEFT_CURLY_BRACKET:  return Token_type::RIGHT_CURLY_BRACKET;
            case Token_type::RIGHT_CURLY_BRACKET: return Token_type::LEFT_CURLY_BRACKET;

            case Token_type::LEFT_TEMPLATE_BRACKET:  return Token_type::RIGHT_TEMPLATE_BRACKET;
            case Token_type::RIGHT_TEMPLATE_BRACKET: return Token_type::LEFT_TEMPLATE_BRACKET;

            case Token_type::LEFT_COMMENT_BRACKET:  return Token_type::RIGHT_COMMENT_BRACKET;
            case Token_type::RIGHT_COMMENT_BRACKET: return Token_type::LEFT_COMMENT_BRACKET;

            default: return Token_type::NULL_TOKEN;
        }
    }

    std::size_t token_width(Token_type token_type) {
        switch (token_type) {
            case Token_type::NULL_TOKEN: return 0;

            case Token_type::LEFT_PARENTHESIS:       return 1;
            case Token_type::RIGHT_PARENTHESIS:      return 1;
            case Token_type::LEFT_SQUARE_BRACKET:    return 1;
            case Token_type::RIGHT_SQUARE_BRACKET:   return 1;
            case Token_type::LEFT_CURLY_BRACKET:     return 1;
            case Token_type::RIGHT_CURLY_BRACKET:    return 1;
            case Token_type::LEFT_TEMPLATE_BRACKET:  return 1;
            case Token_type::RIGHT_TEMPLATE_BRACKET: return 1;
            case Token_type::LEFT_COMMENT_BRACKET:   return 2;
            case Token_type::RIGHT_COMMENT_BRACKET:  return 2;

            case Token_type::PREFIX_PLUS:      return 1;
            case Token_type::PREFIX_MINUS:     return 1;
            case Token_type::PREFIX_INCREMENT: return 2;
            case Token_type::SUFFIX_INCREMENT: return 2;
            case Token_type::PREFIX_DECREMENT: return 2;
            case Token_type::SUFFIX_DECREMENT: return 2;
            case Token_type::SPACED_PERCENT:   return 1;
            case Token_type::SPACED_ASTERISK:  return 1;
            case Token_type::SPACED_PLUS:      return 1;
            case Token_type::SPACED_MINUS:     return 1;
            case Token_type::SPACED_SLASH:     return 1;

            case Token_type::PREFIX_EXCLAMATION: return 1;
            case Token_type::SPACED_LOGICAL_AND: return 2;
            case Token_type::SPACED_LOGICAL_OR:  return 2;
            case Token_type::SPACED_LOGICAL_XOR: return 2;

            case Token_type::PREFIX_TILDE:                  return 1;
            case Token_type::SPACED_AMPERSAND:              return 1;
            case Token_type::SPACED_CARET:                  return 1;
            case Token_type::SPACED_PIPE:                   return 1;
            case Token_type::SPACED_MODULAR_LEFT_SHIFT:     return 2;
            case Token_type::SPACED_MODULAR_RIGHT_SHIFT:    return 2;
            case Token_type::SPACED_SATURATING_LEFT_SHIFT:  return 2;
            case Token_type::SPACED_SATURATING_RIGHT_SHIFT: return 2;

            case Token_type::SPACED_PLUS_EQUALS:  return 2;
            case Token_type::SPACED_MINUS_EQUALS: return 2;
            case Token_type::SPACED_TIMES_EQUALS: return 2;
            case Token_type::SPACED_DIV_EQUALS:   return 2;
            case Token_type::SPACED_REM_EQUALS:   return 2;

            case Token_type::SPACED_BITWISE_AND_EQUALS: return 2;
            case Token_type::SPACED_BITWISE_OR_EQUALS:  return 2;
            case Token_type::SPACED_BITWISE_XOR_EQUALS: return 2;

            case Token_type::SPACED_LOGICAL_AND_EQUALS: return 3;
            case Token_type::SPACED_LOGICAL_OR_EQUALS:  return 3;
            case Token_type::SPACED_LOGICAL_XOR_EQUALS: return 3;

            case Token_type::SPACED_MODULAR_LEFT_SHIFT_EQUALS:     return 3;
            case Token_type::SPACED_MODULAR_RIGHT_SHIFT_EQUALS:    return 3;
            case Token_type::SPACED_SATURATING_LEFT_SHIFT_EQUALS:  return 3;
            case Token_type::SPACED_SATURATING_RIGHT_SHIFT_EQuALS: return 3;

            case Token_type::JOINED_DOUBLE_COLON: return 2;
            case Token_type::JOINED_SINGLE_ARROW: return 2;
            case Token_type::SPACED_DOUBLE_ARROW: return 2;

            case Token_type::SPACED_COMPARE_EQ: return 2;
            case Token_type::SPACED_COMPARE_NE: return 2;
            case Token_type::SPACED_COMPARE_LT: return 2;
            case Token_type::SPACED_COMPARE_GT: return 2;
            case Token_type::SPACED_COMPARE_LE: return 2;
            case Token_type::SPACED_COMPARE_GE: return 2;

            case Token_type::SPACED_EQUALS:        return 1;
            case Token_type::JOINED_DOUBLE_PERIOD: return 2;
            case Token_type::SUFFIX_TRIPLE_PERIOD: return 3;

            case Token_type::SPACED_PERIOD:    return 1;
            case Token_type::COMMA:            return 1;
            case Token_type::AT_SIGN:          return 1;
            case Token_type::HASH:             return 1;
            case Token_type::SUFFIX_AMPERSAND: return 1;
            case Token_type::SUFFIX_CARET:     return 1;

            case Token_type::TEXT:                return SIZE_MAX;
            case Token_type::NUMERIC_LITERAL:     return SIZE_MAX;
            case Token_type::CODEPOINT_LITERAL:   return SIZE_MAX;
            case Token_type::STRING_LITERAL:      return SIZE_MAX;
            case Token_type::SINGLE_LINE_COMMENT: return SIZE_MAX;
            case Token_type::DOC_TEXT:            return SIZE_MAX;

            default: return 0; //TODO: Report internal compiler error
        }
    }

    std::string_view to_string(Token_type token_type) {
        switch (token_type) {
            case Token_type::NULL_TOKEN: return {"NULL_TOKEN"};

            case Token_type::LEFT_PARENTHESIS:       return "LEFT_PARENTHESIS";
            case Token_type::RIGHT_PARENTHESIS:      return "RIGHT_PARENTHESIS";
            case Token_type::LEFT_SQUARE_BRACKET:    return "LEFT_SQUARE_BRACKET";
            case Token_type::RIGHT_SQUARE_BRACKET:   return "RIGHT_SQUARE_BRACKET";
            case Token_type::LEFT_CURLY_BRACKET:     return "LEFT_CURLY_BRACKET";
            case Token_type::RIGHT_CURLY_BRACKET:    return "RIGHT_CURLY_BRACKET";
            case Token_type::LEFT_TEMPLATE_BRACKET:  return "LEFT_TEMPLATE_BRACKET";
            case Token_type::RIGHT_TEMPLATE_BRACKET: return "RIGHT_TEMPLATE_BRACKET";
            case Token_type::LEFT_COMMENT_BRACKET:   return "LEFT_COMMENT_BRACKET";
            case Token_type::RIGHT_COMMENT_BRACKET:  return "RIGHT_COMMENT_BRACKET";

            case Token_type::PREFIX_PLUS:      return "PREFIX_PLUS";
            case Token_type::PREFIX_MINUS:     return "PREFIX_MINUS";
            case Token_type::PREFIX_INCREMENT: return "PREFIX_INCREMENT";
            case Token_type::SUFFIX_INCREMENT: return "SUFFIX_INCREMENT";
            case Token_type::PREFIX_DECREMENT: return "PREFIX_DECREMENT";
            case Token_type::SUFFIX_DECREMENT: return "SUFFIX_DECREMENT";
            case Token_type::SPACED_PERCENT:   return "SPACED_PERCENT";
            case Token_type::SPACED_ASTERISK:  return "SPACED_ASTERISK";
            case Token_type::SPACED_PLUS:      return "SPACED_PLUS";
            case Token_type::SPACED_MINUS:     return "SPACED_MINUS";
            case Token_type::SPACED_SLASH:     return "SPACED_SLASH";

            case Token_type::PREFIX_EXCLAMATION: return "PREFIX_EXCLAMATION";
            case Token_type::SPACED_LOGICAL_AND: return "SPACED_LOGICAL_AND";
            case Token_type::SPACED_LOGICAL_OR:  return "SPACED_LOGICAL_OR";
            case Token_type::SPACED_LOGICAL_XOR: return "SPACED_LOGICAL_XOR";

            case Token_type::PREFIX_TILDE:                  return "PREFIX_TILDE";
            case Token_type::SPACED_AMPERSAND:              return "SPACED_AMPERSAND";
            case Token_type::SPACED_CARET:                  return "SPACED_CARET";
            case Token_type::SPACED_PIPE:                   return "SPACED_PIPE";
            case Token_type::SPACED_MODULAR_LEFT_SHIFT:     return "SPACED_MODULAR_LEFT_SHIFT";
            case Token_type::SPACED_MODULAR_RIGHT_SHIFT:    return "SPACED_MODULAR_RIGHT_SHIFT";
            case Token_type::SPACED_SATURATING_LEFT_SHIFT:  return "SPACED_SATURATING_LEFT_SHIFT";
            case Token_type::SPACED_SATURATING_RIGHT_SHIFT: return "SPACED_SATURATING_RIGHT_SHIFT";

            case Token_type::SPACED_PLUS_EQUALS:  return "SPACED_PLUS_EQUALS";
            case Token_type::SPACED_MINUS_EQUALS: return "SPACED_MINUS_EQUALS";
            case Token_type::SPACED_TIMES_EQUALS: return "SPACED_TIMES_EQUALS";
            case Token_type::SPACED_DIV_EQUALS:   return "SPACED_DIV_EQUALS";
            case Token_type::SPACED_REM_EQUALS:   return "SPACED_REM_EQUALS";

            case Token_type::SPACED_BITWISE_AND_EQUALS: return "SPACED_BITWISE_AND_EQUALS";
            case Token_type::SPACED_BITWISE_OR_EQUALS:  return "SPACED_BITWISE_OR_EQUALS";
            case Token_type::SPACED_BITWISE_XOR_EQUALS: return "SPACED_BITWISE_XOR_EQUALS";

            case Token_type::SPACED_LOGICAL_AND_EQUALS: return "SPACED_LOGICAL_AND_EQUALS";
            case Token_type::SPACED_LOGICAL_OR_EQUALS:  return "SPACED_LOGICAL_OR_EQUALS";
            case Token_type::SPACED_LOGICAL_XOR_EQUALS: return "SPACED_LOGICAL_XOR_EQUALS";

            case Token_type::SPACED_MODULAR_LEFT_SHIFT_EQUALS:     return "SPACED_MODULAR_LEFT_SHIFT_EQUALS";
            case Token_type::SPACED_MODULAR_RIGHT_SHIFT_EQUALS:    return "SPACED_MODULAR_RIGHT_SHIFT_EQUALS";
            case Token_type::SPACED_SATURATING_LEFT_SHIFT_EQUALS:  return "SPACED_SATURATING_LEFT_SHIFT_EQUALS";
            case Token_type::SPACED_SATURATING_RIGHT_SHIFT_EQuALS: return "SPACED_SATURATING_RIGHT_SHIFT_EQuALS";

            case Token_type::JOINED_DOUBLE_COLON: return "JOINED_DOUBLE_COLON";
            case Token_type::JOINED_SINGLE_ARROW: return "JOINED_SINGLE_ARROW";
            case Token_type::SPACED_DOUBLE_ARROW: return "SPACED_DOUBLE_ARROW";

            case Token_type::SPACED_COMPARE_EQ: return "SPACED_COMPARE_EQ";
            case Token_type::SPACED_COMPARE_NE: return "SPACED_COMPARE_NE";
            case Token_type::SPACED_COMPARE_LT: return "SPACED_COMPARE_LT";
            case Token_type::SPACED_COMPARE_GT: return "SPACED_COMPARE_GT";
            case Token_type::SPACED_COMPARE_LE: return "SPACED_COMPARE_LE";
            case Token_type::SPACED_COMPARE_GE: return "SPACED_COMPARE_GE";

            case Token_type::SPACED_EQUALS:        return "SPACED_EQUALS";
            case Token_type::JOINED_DOUBLE_PERIOD: return "JOINED_DOUBLE_PERIOD";
            case Token_type::SUFFIX_TRIPLE_PERIOD: return "SUFFIX_TRIPLE_PERIOD";

            case Token_type::SPACED_PERIOD:    return "SPACED_PERIOD";
            case Token_type::COMMA:            return "COMMA";
            case Token_type::AT_SIGN:          return "AT_SIGN";
            case Token_type::HASH:             return "HASH";
            case Token_type::SUFFIX_AMPERSAND: return "SUFFIX_AMPERSAND";
            case Token_type::SUFFIX_CARET:     return "SUFFIX_CARET";

            case Token_type::TEXT:                return "TEXT";
            case Token_type::NUMERIC_LITERAL:     return "NUMERIC_LITERAL";
            case Token_type::CODEPOINT_LITERAL:   return "CHAR_LITERAL";
            case Token_type::STRING_LITERAL:      return "STRING_LITERAL";
            case Token_type::SINGLE_LINE_COMMENT: return "SINGLE_LINE_COMMENT";
            case Token_type::DOC_TEXT:            return "DOC_TEXT";

            default: return "Unknown";
        }
    }

}
