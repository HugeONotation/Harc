#ifndef HARC_ARCH_X86_64_MICROARCHITECTURES_HPP
#define HARC_ARCH_X86_64_MICROARCHITECTURES_HPP

#include <string_view>
#include <span>
#include <bitset>

namespace harc::arch::x86_64 {

    struct Microarchitecture {

        std::string_view name;

        std::uint8_t num_gpr;
        std::uint8_t num_xmm_reg;
        std::uint8_t num_ymm_reg;
        std::uint8_t num_zmm_reg;

        std::bitset<128> feature_bit_vector;

    };

}

#endif //HARC_ARCH_X86_64_MICROARCHITECTURES_HPP
