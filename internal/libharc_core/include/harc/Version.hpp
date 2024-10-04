#ifndef HARMONIA_VERSION_HPP
#define HARMONIA_VERSION_HPP

#include <cstdint>
#include <string_view>
#include <string>
#include <cstddef>

namespace harc {

    enum class Release_type : std::uint8_t{
        ALPHA = 0,
        BETA = 1,
        PRERELEASE = 2,
        FULL_RELEASE = 3
    };

    ///
    /// \param release Arbitrary Release_type object
    /// \return View over string representation of \p release object.
    [[nodiscard]]
    constexpr std::string_view to_string(Release_type release) {
        switch (release) {
            case Release_type::ALPHA: return "alpha";
            case Release_type::BETA: return "beta";
            case Release_type::PRERELEASE: return "prerelease";
            case Release_type::FULL_RELEASE: return "";
            default: return "Unrecognized release type";
        }
    }

    struct Version {

        //=================================================
        // Instance members
        //=================================================

        Release_type type = Release_type::ALPHA;
        std::uint8_t patch = 0;
        std::uint8_t minor = 0;
        std::uint8_t major = 0;

        //=================================================
        // -ctors
        //=================================================

        constexpr Version(std::uint8_t major, std::uint8_t minor, std::uint8_t patch, Release_type type):
            major(major),
            minor(minor),
            patch(patch),
            type(type) {}

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        constexpr friend auto operator<=>(Version lhs, Version rhs) {
            std::uint32_t v0 = (lhs.major << 24) | (lhs.minor << 16) | (lhs.patch << 8) | std::uint8_t(lhs.type);
            std::uint32_t v1 = (rhs.major << 24) | (rhs.minor << 16) | (rhs.patch << 8) | std::uint8_t(rhs.type);

            return (v0 <=> v1);
        }

        [[nodiscard]]
        constexpr friend bool operator==(Version lhs, Version rhs) = default;

        [[nodiscard]]
        constexpr friend bool operator!=(Version lhs, Version rhs) = default;

        [[nodiscard]]
        constexpr friend bool operator<(Version lhs, Version rhs) = default;

        [[nodiscard]]
        constexpr friend bool operator<=(Version lhs, Version rhs) = default;

        [[nodiscard]]
        constexpr friend bool operator>(Version lhs, Version rhs) = default;

        [[nodiscard]]
        constexpr friend bool operator>=(Version lhs, Version rhs) = default;

    };

    // Ensure that layout of Version struct is consistent
    static_assert(sizeof(Version) == 4);
    static_assert(offsetof(Version, type)  == 0);
    static_assert(offsetof(Version, patch) == 1);
    static_assert(offsetof(Version, minor) == 2);
    static_assert(offsetof(Version, major) == 3);

    ///
    /// \param version Arbitrary Version object
    /// \return String representation of \p version object
    [[nodiscard]]
    inline std::string to_string(Version v) {
        std::string version_string;
        version_string.reserve(24);

        version_string += 'v';
        version_string += std::to_string(v.major);
        version_string += '.';
        version_string += std::to_string(v.minor);
        version_string += '.';
        version_string += std::to_string(v.patch);

        if (v.type != Release_type::FULL_RELEASE) {
            version_string += '-';
            version_string += to_string(v.type);
        }

        return version_string;
    }

    [[nodiscard]]
    inline std::uint32_t to_int(Version v) {
        std::uint32_t ret = 0;

        ret |= std::uint32_t(v.type)  << 0;
        ret |= std::uint32_t(v.patch) << 8;
        ret |= std::uint32_t(v.minor) << 16;
        ret |= std::uint32_t(v.major) << 24;

        return ret;
    }

}

#endif //HARMONIA_VERSION_HPP
