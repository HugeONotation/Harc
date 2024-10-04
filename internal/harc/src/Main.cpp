#include <harc/Harc.hpp>
#include <harc/Logging.hpp>
#include <harc/cli/CLI.hpp>

int main(int argc, char* argv[]) {
    using namespace harc;

    // Parse command line arguments
    cli::Config_results config_results = cli::parse_configuration(argc, argv);
    if (!config_results.successful) {
        harc::print(config_results.message_buffer);
        return EXIT_FAILURE;
    }

    // Return early when further work was not requested at command line
    if (config_results.should_terminate_after_parsing_args) {
        return EXIT_SUCCESS;
    }

    //TODO: Handle a singular

    const Config& config = config_results.config;
    const std::vector<std::string_view>& source_file_paths = config_results.source_paths;

    // Launch core compiler subroutines
    switch (config.network_mode) {
        case Network_mode::LOCAL:       run_locally(source_file_paths, config);     break;
        case Network_mode::SERVER:      run_server(source_file_paths, config);      break;
        case Network_mode::PURE_SERVER: run_pure_server(source_file_paths, config); break;
        case Network_mode::CLIENT:      run_client(config);                         break;
        default: {
            HARC_LOG_CRITICAL("Encountered unrecognized Network_mode enum value in switch statement.");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
