#ifndef HARMONIA_BYTECODE_HPP
#define HARMONIA_BYTECODE_HPP

#include <cstdint>

namespace harc::hvm {

    enum class Op_code: std::uint8_t {
        NO_OP
    };

    enum class Op_type: std::uint8_t {
        VOID,
        UNSIGNED_INT,
        SIGNED_INT,
        FLOAT,
        UNSIGNED_FIXED_POINT,
        SIGNED_FIXED_POINT
    };

    enum class Op_width: std::uint8_t {
        UNKNOWN,
    };

    struct Instruction {
        Op_code op_code;
        Op_type
    };

}

#endif //HARMONIA_BYTECODE_HPP
