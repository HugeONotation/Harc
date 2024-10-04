#include <harc/cli/Help.hpp>
#include <harc/common/Algorithms.hpp>

#include <aul/Span.hpp>

#include <vector>
#include <string_view>
#include <unordered_map>

namespace harc::cli {

    const std::string_view general_help_page =
R"(OVERVIEW: HARC - The Harmonia compiler

USAGE:
  harc [options | file]...
  harc query [args...]

QUERIES:
  --help                Display this information
  --help <topic>        Display information about a particular topic
  --help-topics         Display a list of help topics
  --version             Display version information

OPTIONS:
  --threads <val>       Set the amount of compilation threads
)";

    const std::string_view options_help_page =
R"(
)";

    const std::unordered_map<std::string_view, std::string_view> help_pages {
        {"general", general_help_page},
        {"options", options_help_page},
    };

    std::string_view query_help_page(std::string_view page_name) {
        auto it = help_pages.find(page_name);
        if (it == help_pages.end()) {
            return {};
        }

        return std::get<1>(*it);
    }
    
    const std::vector<std::string_view> page_names{
        "general",
        "options",
    };

    aul::Span<const std::string_view> query_help_topics() {
        return aul::Span<const std::string_view>{page_names.data(), page_names.size()};
    }

    std::array<std::string_view, 3> query_help_page_suggestions(
        std::string_view page_name
    ) {
        constexpr std::size_t max_distance = 4;
        constexpr std::size_t max_results = 3;

        std::array<std::string_view, max_results> ret{};

        std::vector<std::string_view> results = most_similar(
            page_name,
            query_help_topics(),
            levenstein_edit_distance_ascii,
            max_distance,
            max_results
        );

        for (std::size_t i = 0; i < std::min(max_results, results.size()); ++i) {
            ret[i] = results[i];
        }

        return ret;
    }

}
