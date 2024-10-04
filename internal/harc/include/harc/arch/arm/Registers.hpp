#ifndef HARC_ARCH_ARM_REGISTERS_HPP
#define HARC_ARCH_ARM_REGISTERS_HPP

#include <array>
#include <string_view>

namespace harc::arch::arm {

    enum GPR : std::uint8_t {
        NULL_GPR,
        R0, R1, R2, R3, R4, R5, R6, R7,
        R8, R9, R10, R11, R12, R13, R14
    };

    enum S_reg : std::uint8_t {
        NULL_S_REG,
        S0,  S1,  S2,  S3,  S4,  S5,  S6,  S7,
        S8,  S9,  S10, S11, S12, S13, S14, S15,
        S16, S17, S18, S19, S20, S21, S22, S23,
        S24, S25, S26, S27, S28, S29, S30, S31
    };

    enum D_reg : std::uint8_t {
        NULL_D_REG,
        D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,
        D8,  D9,  D10, D11, D12, D13, D14, D15,
        D16, D17, D18, D19, D20, D21, D22, D23,
        D24, D25, D26, D27, D28, D29, D30, D31
    };

    enum Q_reg : std::uint8_t {
        NULL_Q_REG,
        Q0, Q1, Q2,  Q3,  Q4,  Q5,  Q6,  Q7,
        Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15,
    };

    enum Z_reg : std::uint8_t {
        NULL_Z_REG,
        Z0,  Z1,  Z2,  Z3,  Z4,  Z5,  Z6,  Z7,
        Z8,  Z9,  Z10, Z11, Z12, Z13, Z14, Z15,
        Z16, Z17, Z18, Z19, Z20, Z21, Z22, Z23,
        Z24, Z25, Z26, Z27, Z28, Z29, Z30, Z31
    };

    enum P_reg : std::uint8_t {
        NULL_P_REG,
        P0, P1, P2,  P3,  P4,  P5,  P6,  P7,
        P8, P9, P10, P11, P12, P13, P14, P15,
    };

    //Also first fault predicate registers

    //Also scalable vector system control registers

    //=====================================================
    // SIMD Register conversions
    //=====================================================

    inline S_reg to_lo_s_reg();

    inline S_reg to_hi_s_reg();

    inline std::array<S_reg, 2> to_s_regs();

    inline D_reg to_lo_d_reg();

    inline D_reg to_hi_d_reg();

    inline std::array<Neon_D_reg, 2> to_d_regs();

    inline Q_reg to_q_reg();

}

#endif //HARC_ARCH_ARM_REGISTERS_HPP
