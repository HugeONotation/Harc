#include <harc/arch/x86_64/Instruction.hpp>
#include <harc/arch/x86_64/Opcodes.hpp>
#include <harc/arch/x86_64/Opcode_maps.hpp>

#include <algorithm>
#include <span>

namespace harc::arch::x86_64 {

    std::string_view to_string_sib_helper(
        Reg_category reg_category,
        std::uint8_t effective_index,
        bool has_rex_prefix = false
    ) {
        static const std::array<std::span<const std::string_view>, 14> tables{
            std::span<const std::string_view>{gpr8_lo_strings_alt},
            std::span<const std::string_view>{gpr8_lo_strings},
            std::span<const std::string_view>{gpr8_hi_strings},
            std::span<const std::string_view>{gpr16_strings},
            std::span<const std::string_view>{gpr32_strings},
            std::span<const std::string_view>{gpr64_strings},
            std::span<const std::string_view>{st_reg_strings},
            std::span<const std::string_view>{mmx_reg_strings},
            std::span<const std::string_view>{xmm_reg_strings},
            std::span<const std::string_view>{ymm_reg_strings},
            std::span<const std::string_view>{zmm_reg_strings},
            std::span<const std::string_view>{segment_reg_strings},
            std::span<const std::string_view>{control_reg_strings},
            std::span<const std::string_view>{debug_reg_strings}
        };

        auto idx = static_cast<std::uint32_t>(reg_category) + 1;
        idx -= !has_rex_prefix && (reg_category == Reg_category::GP8L);

        return tables[idx][effective_index];
    }

    /*
    std::string to_string(
        SIB_byte sib_byte,
        Reg_category reg_category,
        bool has_rex_prefix,
        bool index3,
        bool base3
    ) {
        std::string ret = "[";
        ret.reserve(12);

        std::string_view scale_str{};
        switch (sib_byte.scale) {
            case 0: scale_str = "1"; break;
            case 1: scale_str = "2"; break;
            case 2: scale_str = "4"; break;
            case 3: scale_str = "8"; break;
        }

        ret += scale_str;

        std::uint8_t effective_index = sib_byte.scale | (index3 << 3);
        std::uint8_t effective_base  = sib_byte.base | (base3 << 3);

        auto index = to_string_sib_helper(reg_category, effective_index, has_rex_prefix);
        auto base = to_string_sib_helper(reg_category, effective_base, has_rex_prefix);




        return ret;
    }
    */

    [[nodiscard]]
    Byte_category categorize_byte_legacy_map0(std::uint8_t byte) {
        return legacy_map0_categories[byte];
    }

    std::optional<Instruction> decode_lock_prefixed_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_rex_encoded_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_rex2_encoded_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_vex2_encoded_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_vex3_encoded_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_evex_encoded_instruction(
        std::span<const std::uint8_t> bytes
    ) {
        //TODO: Implement
    }

    std::optional<Instruction> decode_instruction(std::span<const std::uint8_t> bytes) {
        if (bytes.empty()) {
            return {};
        }

        Instruction ret;

        Byte_category byte0_category = categorize_byte_legacy_map0(bytes[0]);
        if (byte0_category == Byte_category::OPCODE) {

            return ret;
        }

        if (byte0_category == Byte_category::UNUSED) {
            //TODO: Report error
        }

        Instruction instruction{};

        switch (byte0_category) {
            case Byte_category::UNUSED:
                return {}; //TODO: Present error
            case Byte_category::OPCODE:
                break;
                //TODO: Look up opcode properties to populate decoded instruction
            case Byte_category::ESCAPE:
                break; //TODO: Implement
            case Byte_category::SEGMENT:
                break; //TODO: Add segment prefix to list of prefixes
            case Byte_category::REX:
                //TODO: Feed in legacy prefixes
                //TODO: Ensure
                return decode_rex_encoded_instruction(bytes);
            case Byte_category::REX2:
                //TODO: Feed in legacy prefixes
                return decode_rex2_encoded_instruction(bytes);
            case Byte_category::VEX2:
                //TODO: Feed in legacy prefixes
                return decode_vex2_encoded_instruction(bytes);
            case Byte_category::VEX3:
                //TODO: Feed in legacy prefixes
                return decode_vex3_encoded_instruction(bytes);
            case Byte_category::EVEX:
                //TODO: Feed in legacy prefixes
                return decode_evex_encoded_instruction(bytes);
            case Byte_category::OP_SIZE:
                break; //TODO: Implement
            case Byte_category::AD_SIZE:
                break; //TODO: Implement
            case Byte_category::GROUP:
                break; //TODO: Implement
            case Byte_category::LOCK:
                if (instruction.payloads.legacy_prefix_1 != Legacy_prefix1::NONE) {
                    instruction.payloads.legacy_prefix_1 = Legacy_prefix1::LOCK;
                } else {
                    // TODO: Handle error
                }

                break;
                //TODO: Add lock prefix to list of prefixes
            case Byte_category::REPNE:
                if (instruction.payloads.legacy_prefix_1 != Legacy_prefix1::NONE) {
                    instruction.payloads.legacy_prefix_1 = Legacy_prefix1::LOCK;
                } else {
                    // TODO: Handle error
                }


            case Byte_category::REP:
                if (instruction.payloads.legacy_prefix_1 != Legacy_prefix1::NONE) {
                    instruction.payloads.legacy_prefix_1 = Legacy_prefix1::LOCK;
                } else {
                    // TODO: Handle error
                }

                
        }

        return ret;



        /*
        // Extract prefixes
        for (int i = 0; i < max_legacy_prefixes; ++i) {
            auto pos = std::find(prefix_byte_table.begin(), prefix_byte_table.end(), *ptr);
            if (pos != prefix_byte_table.end()) {
                ret.prefixes[i] = static_cast<Legacy_instr_prefix>(pos - prefix_byte_table.begin() + 1);
                ++ptr;
            }
        }

        // Extract opcode
        //TODO: Remove use of magic values
        if (*ptr == 0x0f) {
            ret.opcode |= (ptr[1] << 8) | ptr[0];

            if (*ptr == 0x38 || *ptr == 0x3a) {
                ret.opcode |= ptr[2] << 16;
                ++ptr;
            }
            ++ptr;
        } else {
            ret.opcode = *ptr;
        }
        ++ptr;

        // Extract Mod r/m byte

        // Extract SIB byte

        // Extract Displacement

        // Extract first immediate

        // Extract second immediate

        return ret;
        */
    }

}
