#ifndef HARC_SETTINGS_HPP
#define HARC_SETTINGS_HPP

#include <cstdint>

namespace harc {

    enum class Bounds_checking : std::uint8_t {
        DISABLED,
        ENABLED
    };

    struct Compilation_settings {
        Bounds_checking bounds_checking = Bounds_checking::ENABLED;
    };

}

#endif //HARC_SETTINGS_HPP
