#ifndef HARMONIA_OPERATORS_HPP
#define HARMONIA_OPERATORS_HPP

#include <cstdint>
#include <harc/Logging.hpp>

namespace harc::parser {

    enum class Operator {
        PARENTHESES,

        POST_INCREMENT,
        POST_DECREMENT,
        FUNCTION_CALL,
        SUBSCRIPT,
        SUBVIEW,
        MEMBER_ACCESS,

        PRE_INCREMENT,
        PRE_DECREMENT,
        UNARY_PLUS,
        UNARY_MINUS,
        LOGICAL_NOT,
        BITWISE_NOT,
        UNARY_ASTERISK,
        UNARY_AMPERSAND,

        BINARY_PLUS,
        BINARY_MINUS,
        BINARY_ASTERISK,
        SLASH,
        MODULO,

        LOGICAL_AND,
        LOGICAL_NAND,
        LOGICAL_OR,
        LOGICAL_NOR,
        LOGICAL_XOR,
        LOGICAL_XNOR,

        BITWISE_AND,
        BITWISE_NAND,
        BITWISE_OR,
        BITWISE_NOR,
        BITWISE_XOR,
        BITWISE_XNOR,

        ASSIGN,

        ASSIGN_LOGICAL_AND,
        ASSIGN_LOGICAL_NAND,
        ASSIGN_LOGICAL_OR,
        ASSIGN_LOGICAL_NOR,
        ASSIGN_LOGICAL_XOR,
        ASSIGN_LOGICAL_XNOR,

        ASSIGN_BITWISE_AND,
        ASSIGN_BITWISE_NAND,
        ASSIGN_BITWISE_OR,
        ASSIGN_BITWISE_NOR,
        ASSIGN_BITWISE_XOR,
        ASSIGN_BITWISE_XNOR,

        SHIFT_LEFT,
        SHIFT_RIGHT,

        TRAILING_ELLIPSES,
        RANGE,

        CMP_EQ,
        CMP_NE,
        CMP_LT,
        CMP_LE,
        CMP_GT,
        CMP_GE,
        CMP_TW,

        IN,
        IS,
        AS,

        EQUALS,

        PLUS_EQUALS,
        MINUS_EQUALS,
        ASTERISK_EQUALS,
        SLASH_EQUALS,
        MODULO_EQUALS,

        SHIFT_LEFT_EQUALS,
        SHIFT_RIGHT_EQUALS,
    };


    ///
    /// \param op Operator to check precedence of
    /// \return Precedence of operator. Lower value mean the operator should be
    /// prioritized first.
    [[nodiscard]]
    inline std::uint32_t operator_precedence(Operator op) {
        switch (op) {
            case Operator::PARENTHESES: return 1;

            case Operator::AS: return 2;
            case Operator::IS: return 2;
            case Operator::IN: return 2;

            case Operator::POST_INCREMENT: return 3;
            case Operator::POST_DECREMENT: return 3;
            case Operator::FUNCTION_CALL: return 3;
            case Operator::SUBSCRIPT: return 3;
            case Operator::SUBVIEW: return 3;
            case Operator::MEMBER_ACCESS: return 3;

            case Operator::PRE_INCREMENT: return 4;
            case Operator::PRE_DECREMENT: return 4;
            case Operator::UNARY_MINUS: return 4;
            case Operator::UNARY_PLUS: return 4;
            case Operator::LOGICAL_NOT: return 4;
            case Operator::BITWISE_NOT: return 4;
            case Operator::UNARY_ASTERISK: return 4;
            case Operator::UNARY_AMPERSAND: return 4;

            case Operator::BINARY_ASTERISK: return 5;
            case Operator::SLASH: return 5;
            case Operator::MODULO: return 5;

            case Operator::BINARY_PLUS: return 6;
            case Operator::BINARY_MINUS: return 6;

            case Operator::SHIFT_LEFT: return 7;
            case Operator::SHIFT_RIGHT: return 7;

            case Operator::CMP_EQ: return 8;
            case Operator::CMP_NE: return 8;
            case Operator::CMP_TW: return 8;

            case Operator::CMP_LT: return 9;
            case Operator::CMP_LE: return 9;
            case Operator::CMP_GT: return 9;
            case Operator::CMP_GE: return 9;

            case Operator::BITWISE_AND: return 10;
            case Operator::BITWISE_NAND: return 10;

            case Operator::BITWISE_XOR: return 11;
            case Operator::BITWISE_XNOR: return 11;

            case Operator::BITWISE_OR: return 12;
            case Operator::BITWISE_NOR: return 12;

            case Operator::LOGICAL_AND: return 13;
            case Operator::LOGICAL_NAND: return 13;

            case Operator::LOGICAL_XOR: return 14;
            case Operator::LOGICAL_XNOR: return 14;

            case Operator::LOGICAL_OR: return 15;
            case Operator::LOGICAL_NOR: return 15;

            case Operator::RANGE: return 16;

            case Operator::ASSIGN: return 17;
            case Operator::ASSIGN_LOGICAL_AND: return 17;
            case Operator::ASSIGN_LOGICAL_NAND: return 17;
            case Operator::ASSIGN_LOGICAL_OR: return 17;
            case Operator::ASSIGN_LOGICAL_NOR: return 17;
            case Operator::ASSIGN_LOGICAL_XOR: return 17;
            case Operator::ASSIGN_LOGICAL_XNOR: return 17;
            case Operator::ASSIGN_BITWISE_AND: return 17;
            case Operator::ASSIGN_BITWISE_NAND: return 17;
            case Operator::ASSIGN_BITWISE_OR: return 17;
            case Operator::ASSIGN_BITWISE_NOR: return 17;
            case Operator::ASSIGN_BITWISE_XOR: return 17;
            case Operator::ASSIGN_BITWISE_XNOR: return 17;
            case Operator::TRAILING_ELLIPSES: return 17;

            case Operator::EQUALS: return 18;
            case Operator::PLUS_EQUALS: return 18;
            case Operator::MINUS_EQUALS: return 18;
            case Operator::ASTERISK_EQUALS: return 18;
            case Operator::SLASH_EQUALS: return 18;
            case Operator::MODULO_EQUALS: return 18;
            case Operator::SHIFT_LEFT_EQUALS: return 18;
            case Operator::SHIFT_RIGHT_EQUALS: return 18;

            default:
                HARC_LOG_ERROR("Unrecognized operator enum value");
                return INT32_MAX;
        }
    }

}

#endif //HARMONIA_OPERATORS_HPP
