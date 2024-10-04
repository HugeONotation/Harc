#ifndef HARC_ARCH_X86_64_FLAGS_HPP
#define HARC_ARCH_X86_64_FLAGS_HPP

namespace harc::arch::x86_64 {

    enum class Flag {
        CARRY,
        PARITY,
        AUXILIARY_CARRY,
        ZERO,
        SIGN,
        TRAP,
        INTERRUPT_ENABLE,
        DIRECTION,
        OVERFLOW,
        IO_PRIVILEGE_LEVEL,
        RESUME,
        VIRTUAL_8086,
    };

}

#endif //HARC_ARCH_X86_64_FLAGS_HPP
