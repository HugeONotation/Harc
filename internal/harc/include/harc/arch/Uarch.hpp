#ifndef HARMONIA_UARCH_HPP
#define HARMONIA_UARCH_HPP

#include <string_view>
#include <vector>

#include "Cache.hpp"

namespace harc {

    struct Uarch {
        std::string_view name;

        std::vector<Cache> caches;
    };

}

#endif //HARMONIA_UARCH_HPP
