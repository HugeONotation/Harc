#ifndef HARMONIA_CLI_ARGUMENTS_HPP
#define HARMONIA_CLI_ARGUMENTS_HPP

#include <unordered_map>
#include <string_view>

namespace harc::cli {

    //=========================================================================
    // Exclusive arguments
    //=========================================================================

    using exclusive_response_func = void(*)(int argc, char* const argv[]);

    struct Exclusive_argument {
        exclusive_response_func response_func;
    };

    ///
    /// This map associates argument names to structs representing all relevant
    /// state necessary for populating a harc::Config struct.
    ///
    extern const std::unordered_map<std::string_view, Exclusive_argument> exclusive_argument_map;

    //=========================================================================
    // Named Arguments
    //=========================================================================

    ///
    /// Type of functions that strings passed at the command line
    ///
    using parse_func = void (*)(std::string_view key, std::string_view value, void* out);

    ///
    /// Type of function that ensures that the values parsed from the command
    /// line strings are suitable
    ///
    using validation_func = void (*)(std::string_view key, std::string_view value);

    ///
    /// An argument parsing function that is used for command line arguments
    /// that enable a feature
    ///
    /// \param key Dummy argument
    /// \param value Dummy argument
    /// \param out_ptr Pointer to boolean to set to true
    void parse_nothing_as_true(std::string_view key, std::string_view value, void* out_ptr);

    ///
    /// An argument parsing function that is used for command line arguments
    /// that disable a feature
    ///
    /// \param key Dummy argument
    /// \param value Dummy argument
    /// \param out_ptr Pointer to boolean to set to false
    void parse_nothing_as_false(std::string_view key, std::string_view value, void* out_ptr);

    struct Named_argument {

        ///
        /// Offset into a harc::Config struct at which the parameter exists.
        ///
        /// A value of SIZE_MAX means that this argument is exclusive and
        /// should not be specified in combination with other arguments.
        ///
        std::size_t offset = 0;

        ///
        /// This function pointer is invoked to convert a string representation
        ///
        parse_func parser = nullptr;

        ///
        /// This function pointer is invoked to ensure that the parsed value
        /// is appropriate for the named argument
        ///
        validation_func validator = nullptr;

    };

    ///
    /// This map associates argument names to structs representing all relevant
    /// state necessary for populating a harc::Config struct.
    ///
    extern const std::unordered_map<std::string_view, Named_argument> argument_map;

}

#endif //HARMONIA_CLI_ARGUMENTS_HPP
