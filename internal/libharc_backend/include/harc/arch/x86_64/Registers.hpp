#ifndef HARC_ARCH_X86_64_REGISTERS_HPP
#define HARC_ARCH_X86_64_REGISTERS_HPP

#include <array>
#include <string_view>

namespace harc::arch::x86_64 {

    //=====================================================
    // Register categories
    //=====================================================

    ///
    /// Do not change existing enum values.
    ///
    enum class Reg_category : std::uint8_t {
        NULL_CATEGORY,
        GP8L,
        GP8H,
        GP16,
        GP32,
        GP64,
        ST,
        MMX,
        XMM,
        YMM,
        ZMM,
        SEGMENT,
        CONTROl,
        DEBUG
    };

    //=====================================================
    // 8-bit lo registers
    //=====================================================

    enum GPR8_lo : std::uint8_t {
        NULL_GPR8_LO,
          AL,   CL,   DL,   BL,  SPL,  BPL,  SIL,  DIL,
         R8B,  R9B, R10B, R11B, R12B, R13B, R14B, R15B,
        R16B, R17B, R18B, R19B, R20B, R21B, R22B, R23B,
        R24B, R25B, R26B, R27B, R28B, R29B, R30B, R31B
    };

    constexpr std::array<const std::string_view, 33> gpr8_lo_strings {
        "NULL_GPR8_LO",
          "AL",   "CL",   "DL",   "BL",  "SPL",  "BPL",  "SIL",  "DIL",
         "R8B",  "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B",
        "R16B", "R17B", "R18B", "R19B", "R20B", "R21B", "R22B", "R23B",
        "R24B", "R25B", "R26B", "R27B", "R28B", "R29B", "R30B", "R31B"
    };

    constexpr std::array<const std::string_view, 33> gpr8_lo_strings_alt {
        "NULL_GPR8_LO",
          "AL",   "CL",   "DL",   "BL",   "AH",   "CH",   "DH",   "BH",
         "R8B",  "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B",
        "R16B", "R17B", "R18B", "R19B", "R20B", "R21B", "R22B", "R23B",
        "R24B", "R25B", "R26B", "R27B", "R28B", "R29B", "R30B", "R31B"
    };

    [[nodiscard]]
    inline std::string_view to_string(GPR8_lo reg) {
        return gpr8_lo_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline GPR8_lo parse_gpr8_lo(std::string_view str) {
        for (int i = 0; i < gpr8_lo_strings.size(); ++i) {
            if (gpr8_lo_strings[i] == str) {
                return static_cast<GPR8_lo>(i);
            }
        }

        return NULL_GPR8_LO;
    }

    //=====================================================
    // 8-bit hi registers
    //=====================================================

    enum GPR8_hi : std::uint8_t {
        NULL_GPR8_HI,
          AH,   CH,   DH,   BH,  SPH,  BPH,  SIH,  DIH
    };

    constexpr std::array<const std::string_view, 9> gpr8_hi_strings {
        "NULL_GPR8_HI",
        "AH", "CH", "DH", "BH", "SPH", "BPH", "SIH", "DIH"
    };

    [[nodiscard]]
    inline std::string_view to_string(GPR8_hi reg) {
        return gpr8_hi_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline GPR8_hi parse_gpr8_hi(std::string_view str) {
        for (int i = 0; i < gpr8_hi_strings.size(); ++i) {
            if (gpr8_hi_strings[i] == str) {
                return static_cast<GPR8_hi>(i);
            }
        }

        return NULL_GPR8_HI;
    }

    //=====================================================
    // 16-bit registers
    //=====================================================

    enum GPR16 {
        NULL_GPR16,
          AX,   CX,   DX,   BX,   SP,   BP,   SI,   DI,
         R8W,  R9W, R10W, R11W, R12W, R13W, R14W, R15W,
        R16W, R17W, R18W, R19W, R20W, R21W, R22W, R23W,
        R24W, R25W, R26W, R27W, R28W, R29W, R30W, R31W
    };

    constexpr std::array<const std::string_view, 33> gpr16_strings {
        "NULL_GPR16",
          "AX",   "CX",   "DX",   "BX",   "SP",   "BP",   "SI",   "DI",
         "R8W",  "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W",
        "R16W", "R17W", "R18W", "R19W", "R20W", "R21W", "R22W", "R23W",
        "R24W", "R25W", "R26W", "R27W", "R28W", "R29W", "R30W", "R31W"
    };

    [[nodiscard]]
    inline std::string_view to_string(GPR16 reg) {
        return gpr16_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline GPR16 parse_gpr16(std::string_view str) {
        for (int i = 0; i < gpr16_strings.size(); ++i) {
            if (gpr16_strings[i] == str) {
                return static_cast<GPR16>(i);
            }
        }

        return NULL_GPR16;
    }

    //=====================================================
    // 32-bit registers
    //=====================================================

    enum GPR32 {
        NULL_GPR32,
         EAX,  EBX,  ECX,  EDX,  ESP,  EBP,  ESI,  EDI,
         R8D,  R9D, R10D, R11D, R12D, R13D, R14D, R15D,
        R16D, R17D, R18D, R19D, R20D, R21D, R22D, R23D,
        R24D, R25D, R26D, R27D, R28D, R29D, R30D, R31D
    };

    constexpr std::array<const std::string_view, 33> gpr32_strings {
        "NULL_GPR32",
         "EAX",  "ECX",  "EDX",  "EBX",  "ESP",  "EBP",  "ESI",  "EDI",
         "R8D",  "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D",
        "R16D", "R17D", "R18D", "R19D", "R20D", "R21D", "R22D", "R23D",
        "R24D", "R25D", "R26D", "R27D", "R28D", "R29D", "R30D", "R31D"
    };

    [[nodiscard]]
    inline std::string_view to_string(GPR32 reg) {
        return gpr32_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline GPR32 parse_gpr32(std::string_view str) {
        for (int i = 0; i < gpr32_strings.size(); ++i) {
            if (gpr32_strings[i] == str) {
                return static_cast<GPR32>(i);
            }
        }

        return NULL_GPR32;
    }

    //=====================================================
    // 64-bit registers
    //=====================================================

    enum GPR64 {
        NULL_GPR64,
        RAX,  RBX, RCX, RDX, RSI,  RDI, RBP, RSP,
         R8,  R9,  R10, R11, R12, R13,  R14, R15,
        R16, R17,  R18, R19, R20, R21,  R22, R23,
        R24, R25,  R26, R27, R28, R29,  R30, R31
    };

    constexpr std::array<const std::string_view, 33> gpr64_strings {
        "NULL_GPR64",
        "RAX",  "RBX", "RCX", "RDX", "RSI",  "RDI", "RBP", "RSP",
         "R8",  "R9",  "R10", "R11", "R12", "R13",  "R14", "R15",
        "R16", "R17",  "R18", "R19", "R20", "R21",  "R22", "R23",
        "R24", "R25",  "R26", "R27", "R28", "R29",  "R30", "R31"
    };

    [[nodiscard]]
    inline std::string_view to_string(GPR64 reg) {
        return gpr64_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline GPR64 parse_gpr64(std::string_view str) {
        for (int i = 0; i < gpr32_strings.size(); ++i) {
            if (gpr32_strings[i] == str) {
                return static_cast<GPR64>(i);
            }
        }

        return NULL_GPR64;
    }

    //=====================================================
    // x87 ST registers
    //=====================================================

    enum ST_REG : std::uint8_t {
        NULL_ST_REG,
        ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7
    };

    constexpr std::array<const std::string_view, 9> st_reg_strings {
        "NULL_ST_REG",
        "ST0", "ST1", "ST2", "ST3", "ST4", "ST5", "ST6", "ST7"
    };

    [[nodiscard]]
    inline std::string_view to_string(ST_REG reg) {
        return st_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline ST_REG parse_st_reg(std::string_view str) {
        for (int i = 0; i < st_reg_strings.size(); ++i) {
            if (st_reg_strings[i] == str) {
                return static_cast<ST_REG>(i);
            }
        }

        return NULL_ST_REG;
    }

    //=====================================================
    // MMX registers
    //=====================================================

    enum MMX_REG : std::uint8_t {
        NULL_MMX_REG,
        MMX0, MMX1, MMX2, MMX3, MMX4, MMX5, MMX6, MMX7,
    };

    constexpr std::array<const std::string_view, 9> mmx_reg_strings {
        "NULL_MMX_REG",
        "MMX0", "MMX1", "MMX2", "MMX3", "MMX4", "MMX5", "MMX6", "MMX7"
    };

    [[nodiscard]]
    inline std::string_view to_string(MMX_REG reg) {
        return mmx_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline MMX_REG parse_mmx_reg(std::string_view str) {
        for (int i = 0; i < mmx_reg_strings.size(); ++i) {
            if (mmx_reg_strings[i] == str) {
                return static_cast<MMX_REG>(i);
            }
        }

        return NULL_MMX_REG;
    }

    //=====================================================
    // 128-bit registers
    //=====================================================

    enum XMM_REG : std::uint8_t {
        NULL_XMM_REG,
         XMM0,  XMM1, XMM2,  XMM3,  XMM4,  XMM5,  XMM6,  XMM7,
         XMM8,  XMM9, XMM10, XMM11, XMM12, XMM13, XMM14, XMM15,
        XMM16, XMM17, XMM18, XMM19, XMM20, XMM21, XMM22, XMM23,
        XMM24, XMM25, XMM26, XMM27, XMM28, XMM29, XMM30, XMM31
    };

    constexpr std::array<const std::string_view, 33> xmm_reg_strings {
        "NULL_XMM_REG",
        "XMM0",  "XMM1",  "XMM2",  "XMM3",  "XMM4",  "XMM5",  "XMM6",  "XMM7",
        "XMM8",  "XMM9",  "XMM10", "XMM11", "XMM12", "XMM13", "XMM14", "XMM15",
        "XMM16", "XMM17", "XMM18", "XMM19", "XMM20", "XMM21", "XMM22", "XMM23",
        "XMM24", "XMM25", "XMM26", "XMM27", "XMM28", "XMM29", "XMM30", "XMM31"
    };

    [[nodiscard]]
    inline std::string_view to_string(XMM_REG reg) {
        return xmm_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline XMM_REG parse_xmm_reg(std::string_view str) {
        for (int i = 0; i < xmm_reg_strings.size(); ++i) {
            if (xmm_reg_strings[i] == str) {
                return static_cast<XMM_REG>(i);
            }
        }

        return NULL_XMM_REG;
    }

    //=====================================================
    // 256-bit registers
    //=====================================================

    enum YMM_REG : std::uint8_t{
        NULL_YMM_REG,
        YMM0,  YMM1,  YMM2,  YMM3,  YMM4,  YMM5,  YMM6,  YMM7,
        YMM8,  YMM9,  YMM10, YMM11, YMM12, YMM13, YMM14, YMM15,
        YMM16, YMM17, YMM18, YMM19, YMM20, YMM21, YMM22, YMM23,
        YMM24, YMM25, YMM26, YMM27, YMM28, YMM29, YMM30, YMM31
    };

    constexpr std::array<const std::string_view, 33> ymm_reg_strings {
        "NULL_YMM_REG",
        "YMM0",  "YMM1",  "YMM2",  "YMM3",  "YMM4",  "YMM5",  "YMM6",  "YMM7",
        "YMM8",  "YMM9",  "YMM10", "YMM11", "YMM12", "YMM13", "YMM14", "YMM15",
        "YMM16", "YMM17", "YMM18", "YMM19", "YMM20", "YMM21", "YMM22", "YMM23",
        "YMM24", "YMM25", "YMM26", "YMM27", "YMM28", "YMM29", "YMM30", "YMM31"
    };

    [[nodiscard]]
    inline std::string_view to_string(YMM_REG reg) {
        return ymm_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline YMM_REG parse_ymm_reg(std::string_view str) {
        for (int i = 0; i < ymm_reg_strings.size(); ++i) {
            if (ymm_reg_strings[i] == str) {
                return static_cast<YMM_REG>(i);
            }
        }

        return NULL_YMM_REG;
    }

    //=====================================================
    // 512-bit registers
    //=====================================================

    enum ZMM_REG : std::uint8_t {
        NULL_ZMM_REG,
        ZMM0,  ZMM1,  ZMM2,  ZMM3,  ZMM4,  ZMM5,  ZMM6,  ZMM7,
        ZMM8,  ZMM9,  ZMM10, ZMM11, ZMM12, ZMM13, ZMM14, ZMM15,
        ZMM16, ZMM17, ZMM18, ZMM19, ZMM20, ZMM21, ZMM22, ZMM23,
        ZMM24, ZMM25, ZMM26, ZMM27, ZMM28, ZMM29, ZMM30, ZMM31
    };

    constexpr std::array<const std::string_view, 33> zmm_reg_strings {
        "NULL_ZMM_REG",
        "ZMM0",  "ZMM1",  "ZMM2",  "ZMM3",  "ZMM4",  "ZMM5",  "ZMM6",  "ZMM7",
        "ZMM8",  "ZMM9",  "ZMM10", "ZMM11", "ZMM12", "ZMM13", "ZMM14", "ZMM15",
        "ZMM16", "ZMM17", "ZMM18", "ZMM19", "ZMM20", "ZMM21", "ZMM22", "ZMM23",
        "ZMM24", "ZMM25", "ZMM26", "ZMM27", "ZMM28", "ZMM29", "ZMM30", "ZMM31"
    };

    [[nodiscard]]
    inline std::string_view to_string(ZMM_REG reg) {
        return zmm_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline ZMM_REG parse_zmm_reg(std::string_view str) {
        for (int i = 0; i < zmm_reg_strings.size(); ++i) {
            if (zmm_reg_strings[i] == str) {
                return static_cast<ZMM_REG>(i);
            }
        }

        return NULL_ZMM_REG;
    }

    //=====================================================
    // AVX-512 mask registers
    //=====================================================

    enum Mask_reg : std::uint8_t {
        NULL_MASK_REG,
        K0, K1, K2, K3, K4, K5, K6, K7
    };

    constexpr std::array<const std::string_view, 9> mask_register_strings {
        "NULL_MASK_REG",
        "K0", "K1", "K2", "K3", "K4", "K5", "K6", "K7"
    };

    [[nodiscard]]
    inline std::string_view to_string(Mask_reg reg) {
        return mask_register_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline Mask_reg parse_mask_reg(std::string_view str) {
        for (int i = 0; i < mask_register_strings.size(); ++i) {
            if (mask_register_strings[i] == str) {
                return static_cast<Mask_reg>(i);
            }
        }

        return NULL_MASK_REG;
    }

    //=====================================================
    // Segment registers
    //=====================================================

    enum Segment_reg : std::uint8_t {
        NULL_SEGMENT_REG,
        ES, CS, SS, DS, FS, GS
    };

    constexpr std::array<const std::string_view, 7> segment_reg_strings {
        "NULL_SEGMENT_REG",
        "ES", "CS", "SS", "DS", "FS", "GS"
    };

    [[nodiscard]]
    inline std::string_view to_string(Segment_reg reg) {
        return segment_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline Segment_reg parse_segment_reg(std::string_view str) {
        for (int i = 0; i < segment_reg_strings.size(); ++i) {
            if (segment_reg_strings[i] == str) {
                return static_cast<Segment_reg>(i);
            }
        }

        return NULL_SEGMENT_REG;
    }

    //=====================================================
    // Control registers
    //=====================================================

    enum Control_reg : std::uint8_t {
        NULL_CONTROL_REG,
        CR0, CR1,  CR2,  CR3,  CR4,  CR5,  CR6,  CR7,
        CR8, CR9, CR10, CR11, CR12, CR13, CR14, CR15
    };

    constexpr std::array<const std::string_view, 17> control_reg_strings {
        "NULL_CONTROL_REG",
        "CR0", "CR1",  "CR2",  "CR3",  "CR4",  "CR5",  "CR6",  "CR7",
        "CR8", "CR9", "CR10", "CR11", "CR12", "CR13", "CR14", "CR15"
    };

    [[nodiscard]]
    inline std::string_view to_string(Control_reg reg) {
        return control_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline Control_reg parse_control_reg(std::string_view str) {
        for (int i = 0; i < control_reg_strings.size(); ++i) {
            if (control_reg_strings[i] == str) {
                return static_cast<Control_reg>(i);
            }
        }

        return NULL_CONTROL_REG;
    }

    //=====================================================
    // Debug registers
    //=====================================================

    enum Debug_reg : std::uint8_t {
        NULL_DEBUG_REG,
        DR0, DR1,  DR2,  DR3,  DR4,  DR5,  DR6,  DR7,
        DR8, DR9, DR10, DR11, DR12, DR13, DR14, DR15
    };

    constexpr std::array<const std::string_view, 17> debug_reg_strings {
        "NULL_CONTROL_REG",
        "DR0", "DR1",  "DR2",  "DR3",  "DR4",  "DR5",  "DR6",  "DR7",
        "DR8", "DR9", "DR10", "DR11", "DR12", "DR13", "DR14", "DR15"
    };

    [[nodiscard]]
    inline std::string_view to_string(Debug_reg reg) {
        return debug_reg_strings[static_cast<std::uint8_t>(reg)];
    }

    [[nodiscard]]
    inline Debug_reg parse_debug_reg(std::string_view str) {
        for (int i = 0; i < debug_reg_strings.size(); ++i) {
            if (debug_reg_strings[i] == str) {
                return static_cast<Debug_reg>(i);
            }
        }

        return NULL_DEBUG_REG;
    }

    //=====================================================
    // General register operand
    //=====================================================

    struct Register {
        std::uint16_t category : 4;
        std::uint16_t index : 12;

        Register(GPR8_lo r):
            category(static_cast<int>(Reg_category::GP8L)),
            index(static_cast<int>(r) - 1)
        {}

        Register(GPR8_hi r):
            category(static_cast<int>(Reg_category::GP8H)),
            index(static_cast<int>(r) - 1)
        {}

        Register(GPR16 r):
            category(static_cast<int>(Reg_category::GP16)),
            index(static_cast<int>(r) - 1)
        {}

    };

    static_assert(sizeof(Register) == 2, "class Register was an unexpected size!");

    [[nodiscard]]
    inline std::string_view to_string(Register reg) {
        switch (static_cast<Reg_category>(reg.category)) {
            case Reg_category::NULL_CATEGORY: return {};
            case Reg_category::GP8L: return to_string(static_cast<GPR8_lo>(reg.index));
            case Reg_category::GP8H: return to_string(static_cast<GPR8_hi>(reg.index));
            //TODO: Complete
        }


    }

    //=====================================================
    // GPR conversions
    //=====================================================

    [[nodiscard]]
    inline GPR8_lo to_gpr8_lo(GPR8_lo reg) {
        return static_cast<GPR8_lo>(reg);
    }

    [[nodiscard]]
    inline GPR8_lo to_gpr8_lo(GPR8_hi reg) {
        return static_cast<GPR8_lo>(reg);
    }

    [[nodiscard]]
    inline GPR8_lo to_gpr8_lo(GPR16 reg) {
        return static_cast<GPR8_lo>(reg);
    }

    [[nodiscard]]
    inline GPR8_lo to_gpr8_lo(GPR32 reg) {
        return static_cast<GPR8_lo>(reg);
    }

    [[nodiscard]]
    inline GPR8_lo to_gpr8_lo(GPR64 reg) {
        return static_cast<GPR8_lo>(reg);
    }



    [[nodiscard]]
    inline GPR8_hi to_gpr8_hi(GPR8_lo reg) {
        return static_cast<GPR8_hi>(reg);
    }

    [[nodiscard]]
    inline GPR8_hi to_gpr8_hi(GPR8_hi reg) {
        return static_cast<GPR8_hi>(reg);
    }

    [[nodiscard]]
    inline GPR8_hi to_gpr8_hi(GPR16 reg) {
        return static_cast<GPR8_hi>(reg);
    }

    [[nodiscard]]
    inline GPR8_hi to_gpr8_hi(GPR32 reg) {
        return static_cast<GPR8_hi>(reg);
    }

    [[nodiscard]]
    inline GPR8_hi to_gpr8_hi(GPR64 reg) {
        return static_cast<GPR8_hi>(reg);
    }



    [[nodiscard]]
    inline GPR16 to_gpr16(GPR8_lo reg) {
        return static_cast<GPR16>(reg);
    }

    [[nodiscard]]
    inline GPR16 to_gpr16(GPR8_hi reg) {
        return static_cast<GPR16>(reg);
    }

    [[nodiscard]]
    inline GPR16 to_gpr16(GPR16 reg) {
        return static_cast<GPR16>(reg);
    }

    [[nodiscard]]
    inline GPR16 to_gpr16(GPR32 reg) {
        return static_cast<GPR16>(reg);
    }

    [[nodiscard]]
    inline GPR16 to_gpr16(GPR64 reg) {
        return static_cast<GPR16>(reg);
    }



    [[nodiscard]]
    inline GPR32 to_gpr32(GPR8_lo reg) {
        return static_cast<GPR32>(reg);
    }

    [[nodiscard]]
    inline GPR32 to_gpr32(GPR8_hi reg) {
        return static_cast<GPR32>(reg);
    }

    [[nodiscard]]
    inline GPR32 to_gpr32(GPR16 reg) {
        return static_cast<GPR32>(reg);
    }

    [[nodiscard]]
    inline GPR32 to_gpr32(GPR32 reg) {
        return static_cast<GPR32>(reg);
    }

    [[nodiscard]]
    inline GPR32 to_gpr32(GPR64 reg) {
        return static_cast<GPR32>(reg);
    }



    [[nodiscard]]
    inline GPR64 to_gpr64(GPR8_lo reg) {
        return static_cast<GPR64>(reg);
    }

    [[nodiscard]]
    inline GPR64 to_gpr64(GPR8_hi reg) {
        return static_cast<GPR64>(reg);
    }

    [[nodiscard]]
    inline GPR64 to_gpr64(GPR16 reg) {
        return static_cast<GPR64>(reg);
    }

    [[nodiscard]]
    inline GPR64 to_gpr64(GPR32 reg) {
        return static_cast<GPR64>(reg);
    }

    [[nodiscard]]
    inline GPR64 to_gpr64(GPR64 reg) {
        return static_cast<GPR64>(reg);
    }

    //=====================================================
    // SIMD Register conversions
    //=====================================================

    [[nodiscard]]
    inline XMM_REG to_xmm_reg(XMM_REG reg) {
        return static_cast<XMM_REG>(reg);
    }

    [[nodiscard]]
    inline XMM_REG to_xmm_reg(YMM_REG reg) {
        return static_cast<XMM_REG>(reg);
    }

    [[nodiscard]]
    inline XMM_REG to_xmm_reg(ZMM_REG reg) {
        return static_cast<XMM_REG>(reg);
    }



    [[nodiscard]]
    inline YMM_REG to_ymm_reg(XMM_REG reg) {
        return static_cast<YMM_REG>(reg);
    }

    [[nodiscard]]
    inline YMM_REG to_ymm_reg(YMM_REG reg) {
        return static_cast<YMM_REG>(reg);
    }

    [[nodiscard]]
    inline YMM_REG to_ymm_reg(ZMM_REG reg) {
        return static_cast<YMM_REG>(reg);
    }



    [[nodiscard]]
    inline ZMM_REG to_zmm_reg(XMM_REG reg) {
        return static_cast<ZMM_REG>(reg);
    }

    [[nodiscard]]
    inline ZMM_REG to_zmm_reg(YMM_REG reg) {
        return static_cast<ZMM_REG>(reg);
    }

    [[nodiscard]]
    inline ZMM_REG to_zmm_reg(ZMM_REG reg) {
        return static_cast<ZMM_REG>(reg);
    }

}

#endif //HARC_ARCH_X86_64_REGISTERS_HPP