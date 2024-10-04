#ifndef HARC_ARCH_X86_64_INSTRUCTION_HPP
#define HARC_ARCH_X86_64_INSTRUCTION_HPP

#include "Registers.hpp"

#include <cstdint>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <span>

namespace harc::arch::x86_64 {

    //=====================================================
    // x86_64 prefix info
    //=====================================================

    enum class Legacy_prefix1 : std::uint8_t {
        NONE  = 0x00,
        LOCK  = 0xf0,
        REPNE = 0xf2,
        REP   = 0xf3
    };

    enum class Legacy_prefix2 : std::uint8_t {
        NONE           = 0x00,
        CS_OR_UNLIKELY = 0x2e,
        SS             = 0x36,
        DS_OR_LIKELY   = 0x3e,
        ES             = 0x26,
        FS             = 0x64,
        GS             = 0x65
    };

    enum class Legacy_prefix3 : std::uint8_t {
        NONE         = 0x00,
        OPERAND_SIZE = 0x66
    };

    enum class Legacy_prefix4 : std::uint8_t {
        NONE         = 0x00,
        ADDRESS_SIZE = 0x67
    };

    //=====================================================
    // x86_64 instruction fields info
    //=====================================================

    enum class Addressing_mode : std::uint8_t {
        REGISTER_INDIRECT_ADDRESSING = 0x0,
        SIGNED_BYTE_DISPLACEMENT_ADDRESSING = 0x1,
        SIGNED_DWORD_DISPLACEMENT_ADDRESSING = 0x2,
        REGISTER_ADDRESSING = 0x03
    };

    enum class Imm0_size {
        NONE,
        ONE,
        TWO,
        FOUR,
        EIGHT,
    };

    enum class Code_offset_size {
        ONE,
        TWO,
        FOUR,
        EIGHT,
        TEN
    };

    enum class Scale {
        ONE,
        TWO,
        FOUR,
        EIGHT
    };

    enum class Rounding_mode {
        CURRENT_DIR,
        TRUNCATE,
        UPWARDS,
        DOWNWARDS,
        TO_NEAREST
    };



    //=========================================================================
    // Instruction classes
    //=========================================================================

    struct Instruction_interface {

        Reg_category r_reg : 4 = Reg_category::NULL_CATEGORY;

        Reg_category b_reg : 4 = Reg_category::NULL_CATEGORY;

        Mask_reg mask_reg : 4 = Mask_reg::NULL_MASK_REG;

        Imm0_size imm0_size : 3 = Imm0_size::NONE;

        bool has_imm1 : 1 = false;

        Addressing_mode addressing_mode : 2 = Addressing_mode::REGISTER_INDIRECT_ADDRESSING;

    };

    static_assert(sizeof(Instruction_interface) == 4);


    ///
    /// Struct representing x86 instruction.
    ///
    /// Field are organized in an order that's different than how they should
    /// appear in an assembled instruction to reduce the struct's memory
    /// footprint.
    ///
    struct Instruction_payloads {

        ///
        /// First immediate value
        ///
        std::int64_t imm0 = 0;

        ///
        /// Address displacement
        ///
        std::int32_t displacement = 0;

        Legacy_prefix1 legacy_prefix_1 = Legacy_prefix1::NONE;
        Legacy_prefix2 legacy_prefix_2 = Legacy_prefix2::NONE;
        Legacy_prefix3 legacy_prefix_3 = Legacy_prefix3::NONE;
        Legacy_prefix4 legacy_prefix_4 = Legacy_prefix4::NONE;

        ///
        /// Primary opcode byte
        ///
        std::uint8_t opcode = 0;

        ///
        /// Opcode map index
        ///
        std::uint8_t opcode_map : 5 = 0;

        ///
        /// Extension to opcode
        ///
        std::uint8_t opcode_extension : 3 = 0;

        ///
        /// Second immediate value
        ///
        std::uint8_t imm1{};

        ///
        /// Primary register
        ///
        std::uint8_t r_reg = 0;

        ///
        /// Base register
        ///
        std::uint8_t b_reg = 0;

        ///
        /// Data destination register
        ///
        std::uint8_t d_reg = 0;

        ///
        /// AVX-512 mask register
        ///
        std::uint8_t mask_reg = 0;

        ///
        /// Addressing offset
        ///
        std::uint64_t offset = 0;

    };


    struct Instruction {
        Instruction_payloads payloads;
        Instruction_interface interface;
    };



    ///
    /// \param ptr Pointer to first byte of an x86 instruction.
    /// Incremented by the function to point to one past
    /// the last byte of the instruction
    /// \return Struct representing x86_64 instruction.
    [[nodiscard]]
    std::optional<Instruction> decode_instruction(std::span<const std::uint8_t> bytes);

    ///
    /// \param instruction Arbitrary instruction object
    /// \return String representation of
    [[nodiscard]]
    std::string to_string(Instruction instruction);

}

#endif //HARC_ARCH_X86_64_INSTRUCTION_HPP
