#ifndef HARC_STATUS_CODES_HPP
#define HARC_STATUS_CODES_HPP

#include <string_view>
#include <string>
#include <vector>

namespace harc {

    enum class Error_code : std::int8_t {
        NO_ERROR = 0,
        GENERAL_INTERNAL_ERROR,
        NO_SOURCE_FILES_SPECIFIED,
        INVALID_COMMAND_LINE_ARGUMENTS,
        INCOMPLETE_COMMAND_LINE_PARAMETER,
        INACCESSIBLE_CONFIG_FILE_PATH,
        INVALID_CONFIG_FILE,
        INACCESSIBLE_SOURCE_FILE_PATH,
        INVALID_SOURCE_FILE,
        EXCESSIVE_SOURCE_FILE_LENGTH,
        TOKENIZATION_ERROR,
        PARSING_ERROR,
        SEMANTIC_ANALYSIS_ERROR,
        OPTIMIZATION_ERROR,
        EMISSION_ERROR,
        VALID_ERROR_CODE_THRESHOLD = 126
    };

    ///
    /// \param error Arbitrary Error_code enum value
    /// \return View over string representation of error
    [[nodiscard]]
    std::string_view to_string(Error_code error);

    ///
    /// Struct representing misc. errors in
    ///
    struct Error {
        Error_code error_code = Error_code::NO_ERROR;
        std::string auxiliary_info{};
    };

    /*
    ///
    /// Singleton class representing a buffer containing information
    ///
    /// Use of this class in a multi-threaded context is not safe.
    ///
    class Message_buffer {
    public:

        static void append(Error_code error_code, const std::string& str = {});

        static void append(Error_code error_code, std::string&& str);

        static void append(Error_code error_code, std::string_view str);

        static void append(Error_code error_code, const char* str);

        static void flush();

        static inline bool is_empty() {
            return list.empty();
        }

    private:

        static std::vector<Error> list;

    };
    */

}

#endif //HARC_STATUS_CODES_HPP
