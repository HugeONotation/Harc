#!/usr/bin/env python3

import os
import sys

source_file_template = r'''#include <harc/cli/Help.hpp>
#include <harc/common/Algorithms.hpp>

#include <aul/Span.hpp>

#include <vector>
#include <string_view>
#include <unordered_map>

namespace harc::cli {

//HELP_PAGE_VARIABLE_REPLACEMENT_POINT

    const std::unordered_map<std::string_view, std::string_view> help_pages {
//HELP_PAGE_MAP_REPLACEMENT_POINT
    };

    std::string_view query_help_page(std::string_view page_name) {
        auto it = help_pages.find(page_name);
        if (it == help_pages.end()) {
            return {};
        }

        return std::get<1>(*it);
    }
    
//HELP_PAGE_NAMES_REPLACEMENT_POINT

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
'''

def main():
    help_page_dir = os.path.join(sys.argv[2], 'assets/help_pages')

    # Assuming that the help age directory does not contain anything besides txt files
    # This assumption may break in the future
    help_page_names = sorted(os.listdir(help_page_dir))
    help_page_paths = [os.path.join(help_page_dir, f) for f in help_page_names]

    help_page_contents = []
    for path in help_page_paths:
        with open(path, 'r') as f:
            help_page_contents.append(f.read())

    help_page_variables = ''
    help_page_map_entries = ''
    help_page_name_list = '    const std::vector<std::string_view> page_names{\n'
    for file_name, file_content in zip(help_page_names, help_page_contents):
        file_content = file_content.rstrip('\n')
        file_content += '\n'

        help_page_variables   += '\n\n    const std::string_view ' + file_name + '_help_page =\n' + 'R"(' + file_content + ')"' + ';'
        help_page_map_entries += '\n        {"' + file_name + '", ' + file_name + '_help_page},'
        help_page_name_list += '        "' + file_name + '"' + ',\n'

    help_page_variables = help_page_variables.removeprefix('\n\n')
    help_page_map_entries = help_page_map_entries.removeprefix('\n')
    help_page_name_list += '    };'

    generated_code = source_file_template
    generated_code = generated_code.replace('//HELP_PAGE_VARIABLE_REPLACEMENT_POINT', help_page_variables)
    generated_code = generated_code.replace('//HELP_PAGE_MAP_REPLACEMENT_POINT', help_page_map_entries)
    generated_code = generated_code.replace('//HELP_PAGE_NAMES_REPLACEMENT_POINT', help_page_name_list)


    output_file_path = os.path.join(sys.argv[1], 'Help.cpp')
    with open(output_file_path, 'w') as f:
        f.write(generated_code)


if __name__=="__main__":
    main()
