#ifndef HARMONIA_IR_HPP
#define HARMONIA_IR_HPP

#include <vector>
#include <cstdint>

namespace harc::ir {

    struct Substitution {
        //TODO: Add predicate for checking if substitution applies
    };

    struct Type {

        ///
        /// Amount of bytes which an object of this type occupies
        ///
        std::uint64_t size;

    };

    struct Parameter {
        std::uint32_t type;
    };

    struct Operation {
        std::vector<Parameter> parameters;
    };

    struct Object {
        std::uint32_t lifetime_begin;
        std::uint32_t lifetime_end;

        std::uint32_t type;
    };

    struct Instruction {
        std::uint32_t operation;

        //TODO: Utilize small buffer optimization for the following vectors

        std::vector<std::uint32_t> operands;

        std::vector<std::uint32_t> outputs;
    };

    struct Function {
        std::vector<Instruction> instructions;
        std::vector<Object> objects;

        std::vector<std::uint32_t> types;
        std::vector<std::uint32_t> labels;
    };

    struct Program {

        ///
        /// A list of all functions in the program
        ///
        std::vector<Function> functions;

        ///
        /// A list of all static objects.
        ///
        std::vector<Object> globals_constants;

        ///
        /// A list of all the types which are used by the program
        ///
        std::vector<Type> types;

    };

    [[nodiscard]]
    std::vector<Function> builtin_functions();

    [[nodiscard]]
    std::vector<Type> builtin_types();

}

#endif //HARMONIA_IR_HPP
