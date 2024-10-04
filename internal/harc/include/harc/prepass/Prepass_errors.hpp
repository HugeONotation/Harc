#ifndef HARMONIA_PREPASS_ERRORS_HPP
#define HARMONIA_PREPASS_ERRORS_HPP

#include <cstdint>

namespace harc::prepass {

    enum class Error_code : std::uint8_t {
        NO_ERROR,

        INVALID_UTF8_NON_SHORTEST_FORM,

        INVALID_UTF8_INSUFFICIENT_CONTINUATION_BYTES,
        INVALID_UTF8_EXCESSIVE_CONTINUATION_BYTES,

        INVALID_UTF8_CODEPOINT_IS_SURROGATE_VALUE,
        INVALID_UTF8_CODEPOINT_GREATER_THAN_U10FFFF
    };

    struct Error {
        Error_code error_code = Error_code::NO_ERROR;

        std::uint32_t start_location = 0;
        std::uint32_t end_location = 0;
    };

}

#endif //HARMONIA_PREPASS_ERRORS_HPP
