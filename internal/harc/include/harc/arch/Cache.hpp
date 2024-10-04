#ifndef HARMONIA_CACHE_HPP
#define HARMONIA_CACHE_HPP

#include <cstdint>

namespace harc {

    enum class Cache_type : std::uint8_t {
        UNIFIED_CACHE,
        DATA_CACHE,
        INSTRUCTION_CACHE
    };

    struct Cache {
        Cache_type type = Cache_type::UNIFIED_CACHE;
        std::uint32_t size = 0;
    };

}

#endif //HARMONIA_CACHE_HPP
