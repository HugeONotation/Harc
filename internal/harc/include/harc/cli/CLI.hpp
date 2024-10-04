#ifndef HARC_ARGUMENTS_HPP
#define HARC_ARGUMENTS_HPP

#include <harc/Errors.hpp>
#include <harc/Config.hpp>
#include <harc/Message_buffer.hpp>

#include <memory>
#include <array>
#include <vector>
#include <string_view>

#include <thread>

#include <aul/Span.hpp>

#include <harc/cli/CLI_arguments.hpp>

namespace harc::cli {

    /*
    //=====================================================
    // Error handling
    //=====================================================

    enum class Error_code : std::uint8_t {
        NO_ERROR,
        UNRECOGNIZED_KEY,
        UNRECOGNIZED_VALUE,
        VALUE_IS_TOO_LARGE,
        IPV6_WRONG_NUMBER_OF_BIT_GROUPS,
        IPV6_MULTIPLE_DOUBLE_COLONS,
        IPV4_WRONG_NUMBER_OF_OCTETS,
        IPV4_INVALID_OCTET,
        IPV4_OCTET_VALUE_TOO_LARGE,
        EXCESS_DIGITS,
        INVALID_DIGITS,
        PORT_NOT_AN_INTEGER,
        PORT_NUMBER_TOO_HIGH,
        INACCESSIBLE_ASSEMBLER_PATH,
        DIRECTORY_AS_ASSEMBLER_PATH,
        UNRECOGNIZED_ASSEMBLER_WITHOUT_SPECIFIED_SYNTAX,
        MORE_THREADS_REQUESTED_THAN_AVAILABLE = 128
    };

    std::string_view to_string(Error_code err);

    struct Error {

        //=================================================
        // -ctors
        //=================================================

        explicit Error(
            Error_code code = Error_code::NO_ERROR,
            aul::Span<const std::string_view> suggestions = {}
        ):
            code(code),
            suggestions(suggestions)
        {}

        explicit Error(
            Error_code code,
            std::uint32_t loc_begin,
            std::uint32_t loc_end
        ):
            code(code),
            location_begin(loc_begin),
            location_end(loc_end)
        {}

        //=================================================
        // Instance members
        //=================================================

        Error_code code = Error_code::NO_ERROR;

        std::uint32_t location_begin = 0;
        std::uint32_t location_end = 0;

        aul::Span<const std::string_view> suggestions{};

    };
    */

    //=====================================================
    // Configuration enums
    //=====================================================

    enum class Assembler {
        UNRECOGNIZED_ASSEMBLER,
        GNU_ASSEMBLER,
        NETWIDE_ASSEMBLER,
        LLVM_ASSEMBLER,
        MASM,
        YASM
    };

    enum class Assembly_syntax {
        INTEL,
        ATnT
    };

    enum class Output_format {
        EXECUTABLE,
        ASSEMBLY_FILES,
        C_FILES,
        CPP_FILES
    };

    //=====================================================
    // Configuration state
    //=====================================================

    ///
    /// Structure meant to contain information specified at the command line.
    ///
    struct CLI_arguments {

        ///
        /// Indicates that the command line arguments passed do not involve
        /// invoking the compiler.
        ///
        bool should_terminate_after_parsing_args = false;

        ///
        /// List of source files to be compiled
        ///
        std::vector<std::string_view> source_paths{};

        ///
        /// Number of CPU threads to use for compilation
        ///
        std::uint32_t cpu_worker_thread_count = std::thread::hardware_concurrency();

        ///
        /// Indicates whether the user asked for timing information about the
        /// compilation process.
        ///
        bool is_timing_info_requested = false;


        ///
        /// Role that the program should play in network compilation.
        ///
        Network_mode network_mode = Network_mode::LOCAL;

        bool performance_warnings = true;

        bool style_warnings = true;

        std::array<std::uint16_t, 8> server_address_ipv6{};

        std::array<std::uint8_t, 4> server_address_ipv4{};

        ///
        /// Port that will be used for connecting to instances of Harc
        ///
        ///
        std::uint16_t network_port = 2023;

        ///
        ///
        ///
        Output_format output_format = Output_format::EXECUTABLE;

        ///
        /// Path to assembler to use
        ///
        std::string_view assembler_path{};

        ///
        /// Recognized identity of the assembler at assembler_path
        ///
        Assembler assembler = Assembler::UNRECOGNIZED_ASSEMBLER;

        ///
        /// Syntax of assembly to generate
        ///
        Assembly_syntax assembly_syntax = Assembly_syntax::INTEL;

    };

    ///
    /// Populates the global cli_arguments reference (or rather the
    /// referenced struct)
    ///
    /// \param argc Length of argv array
    /// \param argv List of pointers to command line strings
    /// \return Struct containing information sourced from command line parameters
    void parse_arguments(int argc, char* const argv[]);


    struct Query_results {
        exclusive_response_func response_func = nullptr;
        Message_buffer message_buffer;
    };

    ///
    ///
    /// \param argc Number of command line arguments
    /// \param argv Pointer to list of null-terminated strings
    /// \return Response function if deduced and error messages
    Query_results parse_query(int argc, char* const argv[]);



    ///
    /// Struct containing information necessary for processing command line
    /// arguments
    ///
    struct Config_results {
        Config config;

        Message_buffer message_buffer;
        bool successful = false;

        std::vector<std::string_view> source_paths;

        bool should_terminate_after_parsing_args = false;
    };

    ///
    /// Produce a cli::Config object from command-line arguments
    ///
    /// \param argc Number of command line arguments
    /// \param argv Pointer to command line arguments
    /// \return Configuration and errors encountered during the translation
    /// process.
    Config_results parse_configuration(int argc, char* const argv[]);

}

#endif //HARC_ARGUMENTS_HPP
