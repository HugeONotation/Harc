#ifndef HARMONIA_ARCH_X86_64_EXTENSIONS_HPP
#define HARMONIA_ARCH_X86_64_EXTENSIONS_HPP

namespace harc::x86_64 {

    ///
    /// Enum class representing extensions to the x86-64 architecture.
    ///
    /// Note that integral values are not guaranteed to be stable.
    ///
    enum class Extensions : std::uint8_t {
        NULL_EXTENSION,
        BASE,
        // SSE, and SSE2 deliberately excluded because they're part of the base x86-64 instruction set
        SSE3,
        SSSE3,
        SSE4_1,
        SSE4_2,
        AVX,
        AVX2,
        AVX_F16C,
        AVX_VNNI,
        AVX_VNNI_INT8,
        AVX_NE_CONVERT,
        AVX_IFMA
        FMA
        AVX512F,
        AVX512BW,
        AVX512CD,
        AVX512DQ,
        AVX512ER,
        AVX512IFMA52,
        AVX512PF,
        AVX512VL
        AVX512VPOPCNTDQ,
        AVX512_4FMAPS,
        AVX512_4VNNIW ,
        AVX512_BITALG,
        AVX512_VBMI,
        AVX512_VBMI2,
        AVX512_VNNI,
        AVX512_VP2INTERSECT,
        AVX512_BF16,
        AVX10_1,
        AVX10_2,
        AMX_BF16,
        AMX_INT8,
        AMX_TILE,
        AMX_FP16,
        AMX_COMPLEX,
        ADX,
        AES,
        BMI1,
        BMI2,
        CET_SS,
        CLDEMOTE,
        CLFLUSHOPT,
        CLWB,
        CMPCCXADD,
        CRC32,
        ENQCMD,
        FXSR,
        GFNI,
        INVPCID,
        KEYLOCKER,
        KEYLOCKER_WIDE,
        LZCNT,
        MONITOR,
        MOVBE,
        MOVDIR64B,
        // MPX Deliberately excluded due to being discontinued and never having gained prominence
        PCLMULQDQ,
        PCONFIG,
        POPCNT,
        PREFETCHI,
        PRFCHW,
        ROA_INT,
        RDPID,
        RDRAND,
        RDSSED,
        RDTSCP,
        RTM,
        SERIALIZE,
        SHA,
        TSC,
        TSXLDTRK,
        UINTR,
        VAES,
        VPCLMULQDQ,
        WAITPKG,
        WBONOINVD,
        XSAVE,
        XSAVEC,
        XSAVEOPT,
        XSS
    };

}

#endif //HARMONIA_ARCH_X86_64_EXTENSIONS_HPP
