#include <harc/cli/CLI_arguments.hpp>
#include <harc/cli/CLI.hpp>
#include <harc/cli/Errors.hpp>
#include <harc/cli/Help.hpp>

#include <harc/Harc.hpp>

#include <string>
#include <thread>

#include <fmt/core.h>

#include <climits>

namespace harc::cli {

    //=====================================================
    // Named argument parse functions
    //=====================================================

    void parse_singular_argument(std::string_view key, std::string_view value, void* out_ptr) {
        (void*)&key;
        (void*)&value;
        (void*)&out_ptr;
    }

    void parse_nothing_as_true(std::string_view key, std::string_view value, void* out_ptr) {
        bool& out = *reinterpret_cast<bool*>(out_ptr);
        out = true;
    }

    void parse_nothing_as_false(std::string_view key, std::string_view value, void* out_ptr) {
        bool& out = *reinterpret_cast<bool*>(out_ptr);
        out = false;
    }

    void parse_boolean(std::string_view key, std::string_view value, void* out_ptr) {
        const std::string_view recognized_values[2] = {
            "false",
            "true"
        };

        cli::Message_buffer::append(
            Error_code::UNRECOGNIZED_VALUE,
            key,
            value,
            aul::Span{recognized_values}
        );
    }

    void parse_u16(std::string_view key, std::string_view value, void* out_ptr) {
        std::uint16_t& out = *reinterpret_cast<std::uint16_t*>(out_ptr);
        static_assert(sizeof(unsigned long long) >= sizeof(std::uint16_t));

        unsigned long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U16,
                key,
                value
            );
            return;
        }

        if (UINT16_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U16,
                key,
                value
            );
            return;
        }

        out = v;
    }

    void parse_i16(std::string_view key, std::string_view value, void* out_ptr) {
        std::int16_t& out = *reinterpret_cast<std::int16_t*>(out_ptr);
        static_assert(sizeof(long long) >= sizeof(std::int16_t));

        long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I16,
                key,
                value
            );
            return;
        }

        if (INT16_MIN < v || INT16_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I16,
                key,
                value
            );
            return;
        }

        out = v;
    }

    void parse_u32(std::string_view key, std::string_view value, void* out_ptr) {
        std::uint32_t& out = *reinterpret_cast<std::uint32_t*>(out_ptr);
        static_assert(sizeof(unsigned long long) >= sizeof(std::uint32_t));

        unsigned long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U32,
                key,
                value
            );
            return;
        }

        if (UINT32_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U32,
                key,
                value
            );
            return;
        }

        out = v;
    }

    void parse_i32(std::string_view key, std::string_view value, void* out_ptr) {
        std::int32_t& out = *reinterpret_cast<std::int32_t*>(out_ptr);
        static_assert(sizeof(long long) >= sizeof(std::int32_t));

        long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I32,
                key,
                value
            );
            return;
        }

        if (INT32_MIN < v || INT32_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I32,
                key,
                value
            );
            return;
        }

        out = v;
    }

    void parse_u64(std::string_view key, std::string_view value, void* out_ptr) {
        std::uint64_t& out = *reinterpret_cast<std::uint64_t*>(out_ptr);
        static_assert(sizeof(unsigned long long) >= sizeof(std::uint64_t));

        unsigned long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U64,
                key,
                value
            );
            return;
        }

        if (UINT64_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_U64,
                key,
                value
            );
            return;
        }

        out = v;
    }

    void parse_i64(std::string_view key, std::string_view value, void* out_ptr) {
        std::int64_t& out = *reinterpret_cast<std::int64_t*>(out_ptr);
        static_assert(sizeof(long long) >= sizeof(std::int64_t));

        long long v = 0;
        try {
            v = std::stoull(value.data());
        } catch (std::invalid_argument& e) {
            Message_buffer::append(
                Error_code::VALUE_NOT_UNSIGNED_INTEGER,
                key,
                value
            );
            return;
        } catch (std::out_of_range& e) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I64,
                key,
                value
            );
            return;
        }

        if (INT64_MIN < v || INT64_MAX < v) {
            Message_buffer::append(
                Error_code::VALUE_INT_MAGNITUDE_OUT_OF_RANGE_I64,
                key,
                value
            );
            return;
        }

        out = v;
    }

    //=====================================================
    // Validation functions
    //=====================================================

    void null_validator(std::string_view key, std::string_view value) {
        (void*)&key;
        (void*)&value;
    }

    void validate_port_number(std::string_view key, std::string_view value) {
        /*
        if (arguments.network_port < 1024) {
            Message_buffer::append(Warning_code::PRIVILEGED_PORT_NUMBER, key, value);
        }
        */
    }

    void validate_thread_count(std::string_view key, std::string_view value) {
        /*
        if (arguments.cpu_worker_thread_count == 0 && arguments.network_mode != Network_mode::PURE_SERVER) {
            Message_buffer::append(
                Error_code::ZERO_THREADS_REQUESTED,
                key,
                value
            );
        }

        if (arguments.cpu_worker_thread_count >= std::thread::hardware_concurrency()) {
            Message_buffer::append(
                Warning_code::MORE_THREADS_REQUESTED_THAN_AVAILABLE,
                key,
                value,
                std::to_string(std::thread::hardware_concurrency())
            );
        }
        */
    }

    //=====================================================
    // Response functions
    //=====================================================

    void respond_help(int argc, char* const argv[]) {
        //TODO: Query and print help page
        // The help page that is printed when no help page name is specified is
        // called "general"

        if (argc == 1) {
            //This function should not be called under these circumstances
        }

        if (argc == 2) {
            // Print out the general help page
            // argv[1] should be "--help"
            fmt::print("{}", query_help_page("general"));
        }

        if (argc == 3) {
            // Print out a specific help page
            // argv[1] should be "--help"
            // argv[2] contains the name of the topic to print help for
            std::string_view help_page = query_help_page(argv[2]);
            if (help_page.empty()) {
                auto suggestions = query_help_page_suggestions(argv[2]);

                std::string buffer = fmt::format("\"{}\" is not a recognized help topic.\n", argv[2]);
                buffer.reserve(256);

                if (suggestions[0].empty()) {
                    buffer.append("No suggested help topics");
                }

                if (!suggestions[0].empty()) {
                    buffer.append("  ");
                    buffer.append(suggestions[0]);
                    buffer.push_back('\n');
                }

                if (!suggestions[1].empty()) {
                    buffer.append("  ");
                    buffer.append(suggestions[1]);
                    buffer.push_back('\n');
                }

                if (!suggestions[2].empty()) {
                    buffer.append("  ");
                    buffer.append(suggestions[2]);
                    buffer.push_back('\n');
                }

                fmt::print("{}", buffer);

            } else {
                fmt::print("{}", help_page);
            }
        }

        if (argc >= 4) {
            // Too many arguments were passed
            fmt::print("Too many arguments passed after --help argument");
        }
    }

    void respond_help_topics(int argc, char* const argv[]) {
        auto topic_names = query_help_topics();

        fmt::print("Help topics:\n");
        for (const auto& topic_name : topic_names) {
            fmt::print("  {}\n", topic_name);
        }
    }

    void respond_version(int argc, char* const argv[]) {
        fmt::print(
            "Harc {}\n{}\n{}\n",
            harc::to_string(harc::version),
            "Harmonia compiler",
            "Authored by Huge-O Notation"
        );
    }

    //=====================================================
    // Argument maps
    //=====================================================

    const std::unordered_map<std::string_view, Exclusive_argument> exclusive_argument_map{
        {"help",        {respond_help}},
        {"help_topics", {respond_help_topics}},
        {"version",     {respond_version}}
    };

    const std::unordered_map<std::string_view, Named_argument> argument_map{
        //{"logging_level",         {&args.logging_level,             parse_level}},
        //{"network_mode",          {&args.network_mode,              parse_network_mode}},
        //{"performance_warnings",  {&args.performance_warnings,      parse_boolean, null_validator}},
        //{"max_errors",            {offsetof(Config, max_errors),    parse_u16, null_validator}},
        //{"style_warnings",        {&args.style_warnings,            parse_boolean, null_validator}},
        //{"server_address_ipv6",   {&args.server_address_ipv6,       parse_ipv6_address}},
        //{"server_address_ipv4",   {&args.server_address_ipv4,       parse_ipv4_address}},
        //{"network_port",          {offsetof(Config, network_port),  parse_u16, validate_port_number}},
        //{"assembler",             {&args.assembler_path,            parse_path}},
        {"threads",                 {offsetof(Config, thread_count),  parse_u32, null_validator}}
    };

}
