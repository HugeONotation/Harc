#include <harc/arch/x86_64/Opcodes.hpp>

#include <array>

namespace harc::arch::x86_64 {

    using C = Byte_category;

    const std::array<Byte_category, 256> legacy_map0_categories {
        // 0
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::ESCAPE,

        // 1
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,

        // 2
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::SEGMENT, C::UNUSED,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::SEGMENT, C::UNUSED,

        // 3
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::SEGMENT, C::UNUSED,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::SEGMENT, C::UNUSED,

        // 4
        C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,
        C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,     C::REX,

        // 5
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 6
        C::UNUSED,  C::UNUSED,  C::EVEX,    C::OPCODE,  C::SEGMENT, C::SEGMENT, C::OP_SIZE, C::AD_SIZE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 7
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 8
        C::GROUP,   C::GROUP,   C::GROUP,   C::GROUP,   C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::GROUP,

        // 9
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // A
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // B
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // C
        C::GROUP,   C::GROUP,   C::OPCODE,  C::OPCODE,  C::VEX3,    C::VEX2,    C::GROUP,   C::GROUP,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,

        // D
        C::GROUP,   C::GROUP,   C::GROUP,   C::GROUP,   C::UNUSED,  C::REX2,    C::UNUSED,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // E
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // F
        C::LOCK,    C::OPCODE,  C::REPNE,   C::REP,     C::OPCODE,  C::OPCODE,  C::GROUP,   C::GROUP,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::GROUP,   C::GROUP
    };



    const std::array<Byte_category, 256> legacy_map1_categories {
        // 0
        C::GROUP,   C::GROUP,   C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::UNUSED,  C::OPCODE,  C::UNUSED,  C::OPCODE,

        // 1
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::GROUP,   C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 2
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,
        C::OPCODE,  C::GROUP,   C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 3
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,
        C::ESCAPE,  C::UNUSED,  C::ESCAPE,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,

        // 4
        C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,
        C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,

        // 5
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 6
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,  C::OPCODE,  C::OPCODE,

        // 7
        C::OPCODE,  C::GROUP,   C::GROUP,   C::GROUP,   C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::UNUSED,  C::OPCODE,  C::OPCODE,

        // 8
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // 9
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // A
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::UNUSED,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::GROUP,   C::OPCODE,

        // B
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::GROUP,   C::GROUP,   C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // C
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::GROUP,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // D
        C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // E
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::UNUSED,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,

        // F
        C::UNUSED,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
        C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,  C::OPCODE,
    };





}
