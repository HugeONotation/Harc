#include <harc/Strings.hpp>

namespace harc {

    std::vector<std::string_view> split(
        std::string_view str,
        std::string_view delimiter
    ) {
        std::vector<std::string_view> ret;

        std::size_t pos_start = 0;
        std::size_t pos_end;
        std::size_t delim_len = delimiter.length();
        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            std::string_view token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            if (!token.empty()) {
                ret.push_back(token);
            }
        }

        ret.push_back(str.substr(pos_start));
        return ret;
    }

    std::vector<std::string_view> split(
        std::string_view str,
        char delimiter
    ) {
        std::vector<std::string_view> ret;

        std::size_t pos_start = 0;
        std::size_t pos_end;
        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            std::string_view token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + 1;
            if (!token.empty()) {
                ret.push_back(token);
            }
        }

        ret.push_back(str.substr(pos_start));
        return ret;
    }

    std::string join(std::span<const std::string_view> strings, std::string_view delimiter) {
        if (strings.empty()) {
            return {};
        }

        std::size_t cumulative_delimiter_size = (strings.size() - 1) * delimiter.size();
        std::size_t cumulative_string_size = 0;
        for (auto& str : strings) {
            cumulative_string_size += str.size();
        }

        std::string ret;
        ret.reserve(cumulative_delimiter_size + cumulative_string_size);

        ret.append(strings[0]);
        for (std::size_t i = 1; i < strings.size(); ++i) {
            ret.append(strings[i]);
            ret.append(delimiter);
        }

        return ret;
    }

    std::string replace(
        std::string_view str,
        std::string_view target,
        std::string_view replacement
    ) {
        std::string ret;
        ret.reserve(str.size() + replacement.size());

        std::size_t pos_start = 0;
        std::size_t pos_end;
        while ((pos_end = str.find(target, pos_start)) != std::string::npos) {
            std::string_view token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + 1;
            ret.append(token);
        }

        ret.append(str.substr(pos_start));

        return ret;
    }

}
