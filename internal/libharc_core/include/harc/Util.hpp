#ifndef HARC_UTIL_HPP
#define HARC_UTIL_HPP

#include <cstdint>

namespace harc {

    struct Location {
        std::uint32_t line;
        std::uint32_t column;
    };

}

#endif //HARC_UTIL_HPP
