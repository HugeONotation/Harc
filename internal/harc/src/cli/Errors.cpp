#include <harc/cli/Errors.hpp>

#include <harc/Logging.hpp>

namespace harc::cli {

    std::string_view to_string(Error_code err) {
        switch (err) {
            case Error_code::NO_ERROR: return "";
            case Error_code::UNRECOGNIZED_KEY:
                return "'{key}' is not a recognized command line argument.";
            case Error_code::UNRECOGNIZED_VALUE:
                return "'{value}' is not a recognized value for '{key}'.";
            case Error_code::IPV6_WRONG_NUMBER_OF_BIT_GROUPS:
                return "{value} is not a valid IPv6 address. An IPv6 address should be of the form xxxx:xxxx:xxxx:xxxx.";

            case Error_code::VALUE_NOT_UNSIGNED_INTEGER:
                return "{value} could not be parsed as an unsigned integer.";
            case Error_code::VALUE_NOT_SIGNED_INTEGER:
                return "{value} could not be parsed as a signed integer.";

            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U16:
                return "{value} does not fall into the expected range of [0, 65535]";
            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I16:
                return "{value} does not fall into the expected range of [-32768, 32767]";
            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U32:
                return "{value} does not fall into the expected range of [0, 4294967295]";
            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I32:
                return "{value} does not fall into the expected range of [-2147483648, 2147483647]";
            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U64:
                return "{value} does not fall into the expected range of [0, 18446744073709551615]";
            case Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I64:
                return "{value} does not fall into the expected range of [-9223372036854775808, 9223372036854775807]";

            case Error_code::ZERO_THREADS_REQUESTED:
                return "Zero CPU worker threads requested.";

            default: {
                HARC_LOG_ERROR("Unhandled enum value.");
                return "Missing error description";
            }
        }
    }

    std::string_view to_string(Warning_code warning_code) {
        switch (warning_code) {
            case Warning_code::PRIVILEGED_PORT_NUMBER:
                return "{value} is a privileged port number.";
            case Warning_code::MORE_THREADS_REQUESTED_THAN_AVAILABLE:
                return "{value} is more threads than are available on this system. {extra_info} thread are available.";
            default: {
                HARC_LOG_ERROR("Unhandled enum value.");
                return "Missing error description";
            }
        }
    }

    //=====================================================
    // Message_buffer static state
    //=====================================================

    std::vector<Message> Message_buffer::list;

    //=====================================================
    // Message_buffer static methods
    //=====================================================

    void Message_buffer::append(
        Error_code error_code,
        std::string_view key,
        std::string_view value,
        aul::Span<const std::string_view> suggestions
    ) {
        list.emplace_back(
            error_code,
            Warning_code::NO_WARNING,
            0,
            0,
            key,
            value,
            suggestions,
            std::string{}
        );
    }

    void Message_buffer::append(
        Error_code error_code,
        std::string_view key,
        std::string_view value
    ) {
        list.emplace_back(
            error_code,
            Warning_code::NO_WARNING,
            0,
            0,
            key,
            value,
            aul::Span<const std::string_view>{},
            std::string{}
        );
    }

    void Message_buffer::append(
        Warning_code warning_code,
        std::string_view key,
        std::string_view value
    ) {
        list.emplace_back(
            Error_code::NO_ERROR,
            warning_code,
            0,
            0,
            key,
            value,
            aul::Span<const std::string_view>{},
            std::string{}
        );
    }

    void Message_buffer::append(
        Warning_code warning_code,
        std::string_view key,
        std::string_view value,
        std::string&& str
    ) {
        list.emplace_back(
            Error_code::NO_ERROR,
            warning_code,
            0,
            0,
            key,
            value,
            aul::Span<const std::string_view>{},
            std::move(str)
        );
    }

}
