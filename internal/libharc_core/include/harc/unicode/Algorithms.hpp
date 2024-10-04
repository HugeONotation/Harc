#ifndef HARC_UNICODE_ALGORITHMS_HPP
#define HARC_UNICODE_ALGORITHMS_HPP

#include <string_view>
#include <span>

namespace harc::unicode {

    ///
    /// Computes the number of codepoints in the specified range.
    ///
    /// Assumes that the string view contains valid UTF-8 encoded text.
    ///
    /// \return Number of Unicode codepoints within the specified range
    std::size_t count_codepoints(std::string_view str);

}

#endif //HARC_UNICODE_ALGORITHMS_HPP
