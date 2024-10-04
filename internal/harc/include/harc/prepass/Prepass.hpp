#ifndef HARMONIA_PREPASS_HPP
#define HARMONIA_PREPASS_HPP

#include <harc/Translation_unit.hpp>
#include <harc/prepass/Prepass_errors.hpp>

#include <cstdint>
#include <array>

namespace harc::prepass {

    struct Prepass_results {

        ///
        /// True if all bytes has their MSB cleared
        ///
        bool is_ascii = false;

        ///
        /// Number of codepoints that exist within the
        ///
        std::uint32_t codepoint_count = 0;

        ///
        /// 128-bit hash of source code.
        ///
        /// Computed using MurmurHash3_x86_128 algorithm
        ///
        std::array<std::uint64_t, 2> hash{};

        ///
        /// List of errors encountered while completing pre-pass information
        ///
        std::vector<Error> errors{};

    };

    ///
    /// Extracts all prepass information from source file in a single pass over
    /// the source data.
    ///
    /// \param unit Translation unit to preform prepass on
    /// \return Struct containing results of prepass stage
    Prepass_results prepass(const Translation_unit& unit);

}

#endif //HARMONIA_PREPASS_HPP
