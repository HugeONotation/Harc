#ifndef HARMONIA_CODEPOINT_MANIPULATION_CUH
#define HARMONIA_CODEPOINT_MANIPULATION_CUH

#include <cstdint>

namespace harc::cuda {

    ///
    /// \param codepoint Unicode codepoint to query width of
    /// \return Number of bytes required to encode the specified codepoint
    __device__
    std::uint32_t codepoint_encoding_width(std::uint32_t codepoint);

    ///
    /// Assumes Unicode 15.0
    ///
    /// \param codepoint Unicode codepoint to test
    /// \return True if the codepoint is in the set ID_start.
    __device__
    bool is_in_ID_start(std::uint32_t codepoint);

    /// Assumes Unicode 15.0
    ///
    /// \param codepoint Unicode codepoint to test
    /// \return True if the codepoint is in the set ID_continue
    __device__
    bool is_in_ID_continue(std::uint32_t codepoint);

    ///
    /// \param codepoint Unicode codepoint to test
    /// \return True if the codepoint is a digit
    __device__
    bool is_digit(std::uint32_t codepoint) {
        return ('0' <= codepoint) && (codepoint <= '9');
    }

}

#endif //HARMONIA_CODEPOINT_MANIPULATION_CUH
