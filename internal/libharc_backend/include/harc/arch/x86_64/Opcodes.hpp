#ifndef HARC_ARCH_X86_64_OPCODES_HPP
#define HARC_ARCH_X86_64_OPCODES_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include <optional>

namespace harc::arch::x86_64 {

    struct Opcode_properties {
        std::uint8_t name_index = 0;
        bool has_mod_rm_byte : 1 = false;
        bool has_sib_byte : 1 = false;
        bool has_displacement : 1 = false;
        bool has_imm0 : 1 = false;
        bool has_imm1 : 1 = false;
    };

    static_assert(sizeof(Opcode_properties) == 2);

    ///
    /// 
    ///
    /// \param opcode Target opcode
    /// \return Opcode_properties for target opcode
    [[nodiscard]]
    std::optional<Opcode_properties> get_opcode_properties(std::uint32_t opcode);

    ///
    /// This is a set of enum values which are meant to represent the
    /// additional information contained within prefixes that is necessary to
    /// uniquely identify the semantics of an instruction.
    ///
    enum class Prefix_info : std::uint8_t {
        LEGACY_MAP0, // Legacy map 0
        LEGACY_MAP1, // Legacy map 1
        LEGACY_MAP2, // Legacy map 2
        LEGACY_MAP3, // Legacy map 3

        LOCK,        // Atomic prefix

        REX_W0,
        REX_W1,

        VEX2,

        VEX3_MAP0,
        VEX3_MAP1,
        VEX3_MAP2,
        VEX3_MAP3,
        VEX3_MAP4,

        REX2,
    };

    enum class Operand_info : std::uint8_t {

    };



    ///
    /// This struct represents the combination of an opcode byte and additional
    /// information provided by prefixes such that the semantics of an
    /// instruction may be uniquely identified.
    ///
    struct Composite_opcode {
        Prefix_info opcode_page;
        std::uint8_t opcode_byte;
        std::uint8_t param_info;
    };



    enum class Byte_category : std::uint8_t {
        UNUSED,  // Unused opcode
        OPCODE,  // Single-byte opcode
        ESCAPE,  // Multi-byte escape opcode
        SEGMENT, // Segment override prefix
        REX,     // Rex prefix
        REX2,    // Rex2 prefix
        VEX2,    // Two-byte VEX prefix
        VEX3,    // Three-byte VEX prefix
        EVEX,    // EVEX prefix

        OP_SIZE, // Operand size prefix
        AD_SIZE, // Address size prefix

        GROUP,   // Opcode is extended by Mod r/m byte

        LOCK,    // Lock prefix
        REPNE,   // PRENE prefix
        REP      // REP prefix
    };

}

#endif //HARC_ARCH_X86_64_OPCODES_HPP
