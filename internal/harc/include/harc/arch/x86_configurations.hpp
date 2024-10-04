#ifndef HARMONIA_X86_CONFIGURATIONS_HPP
#define HARMONIA_X86_CONFIGURATIONS_HPP

#include <vector>

#include "Cache.hpp"

namespace harc {



    struct X86_64_cpu {
        std::vector<Cache> caches;

    };

    struct X86_64_arguments {
        bool sse3 = false;
        bool ssse3 = false;
        bool sse4_1 = false;
        bool sse4_2 = false;

        bool avx = false;
        bool avx_f16c = false;
        bool avx_fma = false;
        bool avx2 = false;
        bool avx_vnni = false;
        bool avx_vnni_int8 = false;
        bool avx_ne_convert = false;
        bool avx_ifma = false;

        bool avx512f = false;
        bool avx512bw = false;
        bool avx512cd = false;
        bool avx512dq = false;
        bool avx512er = false;
        bool avx512ifma52 = false;
        bool avx512pf = false;
        bool avx512vl = false;
        bool avx512vpopcntdq = false;
        bool avx512_4fmaps = false;
        bool avx512_4vnniw = false;
        bool avx512bf16 = false;
        bool avx512bitalg = false;
        bool avx512vbmi = false;
        bool avx512vbmi2 = false;
        bool avx512vnni = false;
        bool avx512vp2intersect = false;
        bool avx512fp16 = false;
        bool avx10_1 = false;
        bool avx10_2 = false;

        bool amx_bf16 = false;
        bool amx_int8 = false;
        bool amx_file = false;
        bool amx_fp16 = false;
        bool amx_complex = false;

        bool adx = false;
        bool aes = false;
        bool bmi = false;
        bool bmi2 = false;
        bool cet_ss = false;
        bool cldemote = false;
        bool clflushopt = false;
        bool clwb = false;
        bool cmpccxadd = false;
        bool crc32 = false;
        bool enqcmd = false;
        bool fsgbase = false;
        bool fxsr = false;
        bool gfni = false;
        bool hreset = false;
        bool invpcid = false;
        bool keylocker = false;
        bool keylocker_wide = false;
        bool lzcnt = false;
        bool monitor = false;
        bool movbe = false;
        bool movdir64b = false;
        bool movdiri = false;
        bool mpx = false;
        bool pclmulqdq = false;
        bool pconfig = false;
        bool popcnt = false;
        bool prefetchi = false;
        bool prefetchwt1 = false;
        bool prfchw = false;
        bool rao_int = false;
        bool rdpid = false;
        bool rdrand = false;
        bool rdseed = false;
        bool rdtscp = false;
        bool rtm = false;
        bool serialize = false;
        bool sha = false;
        bool tsc = false;
        bool tsxldtrk = false;
        bool uintr = false;
        bool vaes = false;
        bool vpclmulqdq = false;
        bool waitpkg = false;
        bool wbnoinvd = false;
        bool xsave = false;
        bool xsavec = false;
        bool xsaveopt = false;
        bool xss = false;
    };

}

#endif //HARMONIA_X86_CONFIGURATIONS_HPP
