#ifndef HARC_CONFIG_HPP
#define HARC_CONFIG_HPP

namespace harc {

    enum class Char_set {
        UTF8,
        ASCII
    };

    enum class Level {
        LOW,
        MEDIUM,
        HIGH
    };

    struct Config {

        ///
        /// Character set for source code.
        ///
        /// Specifying ASCII amy allow for faster operation of the front end.
        ///
        Char_set character_set = Char_set::UTF8;

        ///
        ///
        ///

    };

}

#endif //HARC_CONFIG_HPP
