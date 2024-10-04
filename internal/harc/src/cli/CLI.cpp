#include <harc/cli/CLI.hpp>
#include <harc/cli/CLI_arguments.hpp>

#include <harc/Logging.hpp>

#include <aul/containers/Array_map.hpp>

#include <fmt/format.h>

#include <sstream>
#include <span>

namespace harc::cli {

    Config_results config_string_map_to_config_struct(
        aul::Array_map<std::string_view, std::string_view>& config_map
    ) {
        Config_results ret;

        // Iterate through configuration map, running parsers and validators
        for (const auto& key_value_pair : config_map) {
            const auto& [key, value] = key_value_pair;

            // Retrieve argument parsers and validators
            auto it = argument_map.find(key);
            if (it == argument_map.end()) {
                // Handle unrecognized command line parameters
                ret.message_buffer.error(fmt::format("Unrecognized command line parameter: {}", "key"));

                //TODO: Handle singular arguments

                continue;
            }

            auto [offset, parser, validator] = std::get<1>(*it);

            // Parse argument
            void* parameter_location = reinterpret_cast<char*>(&ret.config) + offset;
            parser(key, value, parameter_location);

            // Validate argument
            if (validator) {
                validator(key, value);
            }
        }

        ret.successful = true;
        return ret;
    }

    harc::Error_code post_validation_argument_processing() {
        namespace fs = std::filesystem;

        /*
        // Validate assembler path
        if (arguments.assembler_path.empty()) {
            // Use default choice of assembler
            #if HARC_POSIX
            impl::cli_args.assembler_path = "/usr/bin/as";
            #else
            static_assert(false, "Default assembler path not specified for target operating system.")
            #endif
        }

        if (!fs::exists(arguments.assembler_path)) {

        } else if (fs::is_directory(arguments.assembler_path)) {

        } else {

        }
        */

        return harc::Error_code::NO_ERROR;
    }

    Query_results parse_query(int argc, char* const argv[]) {
        Query_results ret;

        // Handle no query specified
        if (argc == 1) {
            
        }

        return ret;
    }

    Config_results parse_configuration(int argc, char* const argv[]) {
        aul::Array_map<std::string_view, std::string_view> cli_config_map{};

        Config_results results{};

        // Handle no options specified
        if (argc == 1) {
            results.message_buffer.error("No source files specified.");
            return results;
        }

        //TODO: Do special handling for exclusive arguments

        // Iterate through command line arguments, skipping over the executable name
        for (unsigned i = 1; i < argc; ++i) {
            std::string_view curr{argv[i]};
            std::string_view next{};

            if ((i + 1) < argc) {
                next = std::string_view{argv[i + 1]};
            }

            //TODO: Handling of exclusive arguments.

            if (curr.starts_with("--")) {
                auto it = exclusive_argument_map.find(curr.substr(2));
                if (it != exclusive_argument_map.end()) {
                    //Invoke the appropriate response function
                    it->second.response_func(argc, argv);
                }
            }

            if (curr.starts_with("--")) {
                // Read as full-length parameter

                parse_func argument_parser = argument_map.at(curr.substr(2)).parser;

                // Note: The offset value for an exclusive argument is SIZE_MAX
                std::size_t offset = argument_map.at(curr.substr(2)).offset;

                //Handle exclusive arguments
                if (offset == SIZE_MAX && i != 1) {
                    //TODO: Report error
                    results.message_buffer.error("Could not organize the ");
                }

                bool takes_value = true;
                takes_value &= (argument_parser != parse_nothing_as_true);
                takes_value &= (argument_parser != parse_nothing_as_false);
                takes_value &= offset != SIZE_MAX;
                takes_value &= !next.empty();
                takes_value &= !next.starts_with("-");

                std::string_view argument_key{argv[i] + 2};
                std::string_view argument_value{};

                if (takes_value) {
                    argument_value = argv[i + 1];
                    ++i; // Skip over next command line string
                }

                cli_config_map.emplace(argument_key, argument_value);
            } else if (curr.starts_with("-")) {
                // Read as abbreviated parameter
                cli_config_map.emplace(
                    std::string_view{argv[i] + 1},
                    std::string_view{}
                );
            } else {
                // Read as source file path
                results.source_paths.emplace_back(argv[i]);
            }
        }

        results = config_string_map_to_config_struct(cli_config_map);
        return results;
    }

}
