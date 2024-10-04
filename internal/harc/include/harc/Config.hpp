#ifndef HARC_CONFIG_HPP
#define HARC_CONFIG_HPP

#include <harc/lexer/Lexer.hpp>
#include <harc/parser/Parser.hpp>

#include <string_view>
#include <cstdio>
#include <cstdint>
#include <thread>
#include <array>
#include <span>

namespace harc {

    //=====================================================
    // Error printing callbacks
    //=====================================================

    using message_callback = void (*)(
        std::string_view message_type,
        std::string_view message
    );

    using message_with_path_callback = void(*) (
        std::string_view message_type,
        std::string_view path,
        std::string_view message
    );

    using messsage_with_path_location_callback = void(*) (
        std::string_view message_type,
        std::string_view path,
        std::array<std::uint32_t, 2>,
        std::string_view message
    );

    inline void stdout_callback(
        std::string_view message_type,
        std::string_view message
    ) {
        std::string out{message_type};
        out += ": ";
        out += message;
        fprintf(stdout, "%s", out.data());
    }

    inline void stderr_callback(
        std::string_view message_type,
        std::string_view message
    ) {
        std::string out{message_type};
        out += ": ";
        out += message;
        fprintf(stderr, "%s", out.data());
    }

    //=====================================================
    // Configuration enums
    //=====================================================

    ///
    /// LOCAL - The compiler is run on the local machine
    ///
    /// SERVER - The program launches a harc server. The program will also
    /// launch a client.
    ///
    /// PURE_SERVER - The program launches a Harc server but does not launch a
    /// client
    ///
    /// CLIENT - The program launches a Harc client.
    ///
    enum class Network_mode {
        LOCAL,
        SERVER,
        PURE_SERVER,
        CLIENT
    };

    ///
    ///
    ///
    enum class Level {
        NONE,
        LOW,
        MEDIUM,
        HIGH
    };

    ///
    /// DISABLED - No caching is performed. Any existing cache is neither read
    /// from nor written to. No cache will be created after the current build.
    ///
    /// TIMESTAMP - Cache entries will be marked as invalid if the file
    /// modification timestamps on the source files do not match the
    /// corresponding timestamps stored within the cache index.
    ///
    /// HASH - Cache entries will be marked as invalid if hashes of the source
    /// files do not match those stored within the cache index.
    ///
    /// TIMESTAMP_AND_HASH - Cache entries will be marked as invalid if either
    /// the file modification timestamps on the source file do not match the
    /// corresponding timestamps stored within teh cache index or if hashes of
    /// the source files do not match those stored within the cache index.
    ///
    enum class Caching_policy {
        DISABLED,
        TIMESTAMP,
        HASH,
        TIMESTAMP_AND_HASH
    };



    struct Config {

        //=================================================
        // Caching configuration
        //=================================================

        ///
        /// Path to directory into which the build cache should read from and
        /// where it should be placed.
        ///
        std::string_view cache_directory = "harc_build_cache";

        ///
        /// Controls the caching strategy used by the build
        ///
        Caching_policy caching_policy = Caching_policy::TIMESTAMP_AND_HASH;

        //=================================================
        // Error printing configuration
        //=================================================

        message_callback print_info_callback = stdout_callback;

        message_callback print_warning_callback = stdout_callback;

        message_callback print_error_callback = stderr_callback;

        std::uint32_t max_errors = 100;

        //=================================================
        // Misc. config details
        //=================================================

        ///
        /// Number of threads to use for compilation on the current machine.
        ///
        std::uint16_t thread_count = std::thread::hardware_concurrency();

        #if HARC_USE_CUDA
        ///
        /// Indices of CUDA devices to use for compilation
        ///
        std::vector<std::uint32_t> cuda_device_indices;
        #endif

        ///
        /// Amount of information to report about the amount of time taken by
        /// the compiler on each compilation stage.
        ///
        /// NONE - No information will be reported.
        /// LOW - Information about total compilation time will be reported
        /// MEDIUM - Information about cumulative time spent on each stage will
        /// be reported, as well as total time.
        /// HIGH - Same as MEDIUM
        ///
        Level stage_timing_data_report = Level::NONE;

        ///
        /// Amount of information to report about the amount of time taken to
        /// compile each file.
        ///
        /// NONE - No information will be reported
        /// LOW - Information about total time spent processing each file will
        /// be reported.
        /// MEDIUM - Information about time spent processing each file will be
        /// reported on a per-stage basis.
        /// HIGH - Same as MEDIUM
        ///
        Level file_timing_data_report = Level::NONE;

        //=================================================
        // Networking configuration
        //=================================================

        ///
        /// Method for interacting with network
        ///
        Network_mode network_mode = Network_mode::LOCAL;

        ///
        /// Port number to use for compiling other
        ///
        std::uint16_t network_port = 2023;

        ///
        /// ipv4 address of server to connect to.
        ///
        std::array<std::uint8_t, 4> server_ipv4_address;

        ///
        /// ipv6 address of server to connect to
        ///
        std::array<std::uint16_t, 8> server_ipv6_address;

    };

}

#endif //HARC_CONFIG_HPP
