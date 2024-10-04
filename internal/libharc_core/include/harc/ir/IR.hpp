#ifndef HARC_IR_IR_HPP
#define HARC_IR_IR_HPP

#include <vector>
#include <cstdint>

#include "Operations.hpp"

namespace harc::ir {

    struct HIR_program {
        std::vector<std::uint32_t> operations;
        std::vector<std::uint32_t> instructions;
        std::vector<std::uint32_t> types;
    };

}

#endif //HARC_IR_IR_HPP
