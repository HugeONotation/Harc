#ifndef HARMONIA_CLI_ERRORS_HPP
#define HARMONIA_CLI_ERRORS_HPP

#include <cstdint>
#include <string_view>

#include <aul/Span.hpp>
#include <vector>

namespace harc::cli {

    //=====================================================
    // Error handling
    //=====================================================

    enum class Error_code : std::uint8_t {
        NO_ERROR,
        UNRECOGNIZED_KEY,
        UNRECOGNIZED_VALUE,

        VALUE_NOT_UNSIGNED_INTEGER,
        VALUE_NOT_SIGNED_INTEGER,

        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U16,
        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I16,
        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U32,
        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I32,
        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U64,
        VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I64,

        ZERO_THREADS_REQUESTED,

        IPV6_WRONG_NUMBER_OF_BIT_GROUPS,
        IPV6_MULTIPLE_DOUBLE_COLONS,
        IPV4_WRONG_NUMBER_OF_OCTETS,
        IPV4_INVALID_OCTET,
        IPV4_OCTET_VALUE_TOO_LARGE,
        EXCESS_DIGITS,
        INVALID_DIGITS,
        //INACCESSIBLE_ASSEMBLER_PATH,
        //DIRECTORY_AS_ASSEMBLER_PATH,
        //UNRECOGNIZED_ASSEMBLER_WITHOUT_SPECIFIED_SYNTAX,
    };

    ///
    /// \param err Arbitrary Error_code value
    /// \return View over string representation of error code
    std::string_view to_string(Error_code err);

    enum class Warning_code : std::uint8_t {
        NO_WARNING,
        PRIVILEGED_PORT_NUMBER,
        MORE_THREADS_REQUESTED_THAN_AVAILABLE,
    };

    ///
    /// \param err Arbitrary Warning_code value
    /// \return View over string representation of warning code
    std::string_view to_string(Warning_code err);

    struct Message {
        Error_code error_code = Error_code::NO_ERROR;
        Warning_code warning_code = Warning_code::NO_WARNING;

        std::uint32_t location_begin = 0;
        std::uint32_t location_end = 0;

        std::string_view key;
        std::string_view value;

        aul::Span<const std::string_view> suggestions{};

        std::string extra_info;
    };

    class Message_buffer {
    public:

        static void append(
            Error_code error_code,
            std::string_view key,
            std::string_view value,
            aul::Span<const std::string_view> suggestions
        );

        static void append(
            Error_code error_code,
            std::string_view key,
            std::string_view value
        );

        static void append(
            Warning_code code,
            std::string_view key,
            std::string_view value
        );

        static void append(
            Warning_code code,
            std::string_view key,
            std::string_view value,
            std::string&& str
        );

    private:

        static std::vector<Message> list;

    };

}

#endif //HARMONIA_CLI_ERRORS_HPP
