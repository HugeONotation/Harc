#ifndef HARC_ARCH_X86_64_OPCODE_MAP_HPP
#define HARC_ARCH_X86_64_OPCODE_MAP_HPP

#include "Opcodes.hpp"

#include <array>

namespace harc::arch::x86_64 {

    extern const std::array<Byte_category, 256> legacy_map0_categories;

    extern const std::array<Byte_category, 256> legacy_map1_categories;
    extern const std::array<Byte_category, 256> legacy_map1_66_categories;
    extern const std::array<Byte_category, 256> legacy_map1_f2_categories;
    extern const std::array<Byte_category, 256> legacy_map1_f3_categories;

    extern const std::array<Byte_category, 256> legacy_map2_categories;
    extern const std::array<Byte_category, 256> legacy_map2_66_categories;
    extern const std::array<Byte_category, 256> legacy_map2_f2_categories;
    extern const std::array<Byte_category, 256> legacy_map2_f3_categories;
    extern const std::array<Byte_category, 256> legacy_map2_66_f3_categories;

    extern const std::array<Byte_category, 256> legacy_map3_categories;
    extern const std::array<Byte_category, 256> legacy_map3_66_categories;
    extern const std::array<Byte_category, 256> legacy_map3_f2_categories;

}

#endif //HARC_ARCH_X86_64_OPCODE_MAP_HPP
