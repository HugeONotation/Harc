#include <harc/common/Algorithms.hpp>

#include <vector>

namespace harc {

    std::size_t levenstein_edit_distance_ascii(
        std::string_view str0,
        std::string_view str1,
        std::size_t max
    ) {
        // Return immediately if the difference in length is excessive
        std::size_t length_diff = std::abs(std::ptrdiff_t(str0.size() - str1.size()));
        if (length_diff > max) {
            return max;
        }

        // Return immediately if the difference in organization is too long
        std::size_t diff_cnt = length_diff;
        std::vector<std::uint32_t> row;
        row.resize(str0.size() + 1);

        for (int j = 0; j < str0.size() + 1; ++j) {
            row[j] = j;
        }

        for (int i = 1; i < str1.size() + 1; ++i) {
            std::uint32_t upper_left_cell_cost = i - 1;
            std::uint32_t left_cell_cost = i;

            for (int j = 1; j < str0.size() + 1; ++j) {
                std::uint32_t cell_cost = 0;

                if (str0[j - 1] == str1[i - 1]) {
                    cell_cost = upper_left_cell_cost;
                } else {
                    std::uint32_t c0 = left_cell_cost;
                    std::uint32_t c1 = upper_left_cell_cost;
                    std::uint32_t c2 = row[j];

                    cell_cost = 1 + std::min(c0, std::min(c1, c2));
                }

                upper_left_cell_cost = row[j];
                left_cell_cost = cell_cost;
                row[j] = cell_cost;
            }
        }

        return row.back();
    }

    std::size_t levenstein_edit_distance_utf8(
        std::string_view str0,
        std::string_view str1,
        std::size_t max = SIZE_MAX
    ) {
        //TODO:Implement
        return 0;
    }

    std::size_t damerau_levenstein_edit_distance_ascii(
        std::string_view str0,
        std::string_view str1,
        std::size_t max
    ) {
        //TODO: Optimize implementation to only use a single row of the table

        // Return immediately if the difference in length is excessive
        std::size_t length_diff = std::abs(std::ptrdiff_t(str0.size() - str1.size()));
        if (length_diff > max) {
            return max;
        }

        if (str0.empty()) {
            return str1.size();
        }

        if (str1.empty()) {
            return str0.size();
        }

        std::size_t width = str0.size() + 1;
        std::size_t height = str1.size() + 1;

        std::vector<std::uint32_t> table;
        table.resize(width * height);

        auto compute_index = [width] (std::size_t y, std::size_t x) -> std::uint32_t {
            return y * width + x;
        };

        // Populate top row
        for (std::size_t i = 0; i < width; ++i) {
            table[compute_index(0, i)] = i;
        }

        // Populate left column
        for (std::size_t i = 1; i < height; ++i) {
            table[compute_index(i, 0)] = i;
        }

        // Populate second row
        for (std::size_t j = 1; j < width; ++j) {
            std::size_t i = 1;

            std::uint32_t cell_cost = 0;

            if (str0[j - 1] == str1[i - 1]) {
                // Equal chars
                cell_cost = table[compute_index(i - 1, j - 1)];
            }  else {
                std::uint32_t c0 = table[compute_index(i - 0, j - 1)];
                std::uint32_t c1 = table[compute_index(i - 1, j - 1)];
                std::uint32_t c2 = table[compute_index(i - 1, j - 0)];

                cell_cost = 1 + std::min(c0, std::min(c1, c2));
            }

            table[compute_index(1, i)] = cell_cost;
        }

        // Populate second column
        for (std::size_t i = 2; i < height; ++i) {
            std::size_t j = 1;

            std::uint32_t cell_cost = 0;

            if (str0[j - 1] == str1[i - 1]) {
                // Equal chars
                cell_cost = table[compute_index(i - 1, j - 1)];
            }  else {
                std::uint32_t c0 = table[compute_index(i - 0, j - 1)];
                std::uint32_t c1 = table[compute_index(i - 1, j - 1)];
                std::uint32_t c2 = table[compute_index(i - 1, j - 0)];

                cell_cost = 1 + std::min(c0, std::min(c1, c2));
            }

            table[compute_index(i, 1)] = cell_cost;
        }

        // Fill rest of table
        for (std::size_t i = 2; i < str1.size() + 1; ++i) {
            for (std::size_t j = 2; j < str0.size() + 1; ++j) {
                std::uint32_t cell_cost = 0;

                if (str0[j - 1] == str1[i - 1]) {
                    // Equal chars
                    cell_cost = table[compute_index(i - 1, j - 1)];
                } else if (str0[j - 1] == str1[i - 2] && str0[j - 2] == str1[i - 1]) {
                    // Adjacent chars transposed
                    cell_cost = table[compute_index(i - 1, j - 1)];
                } else {
                    std::uint32_t c0 = table[compute_index(i - 0, j - 1)];
                    std::uint32_t c1 = table[compute_index(i - 1, j - 1)];
                    std::uint32_t c2 = table[compute_index(i - 1, j - 0)];

                    cell_cost = 1 + std::min(c0, std::min(c1, c2));
                }

                table[compute_index(i, j)] = cell_cost;
            }
        }

        return std::max(static_cast<std::size_t>(table.back()), max);
    }

    std::size_t demerau_levenstein_edit_distance_utf8(
        std::string_view str0,
        std::string_view str1
    ) {
        //TODO: Implement
        return 0;
    }

    struct Rank_string_view_pair {
        std::size_t distance;
        std::string_view view;

        friend bool operator<(
            const Rank_string_view_pair& lhs,
            const Rank_string_view_pair& rhs
        ) {
            return lhs.distance < rhs.distance;
        }
    };

    std::vector<std::string_view> most_similar(
        std::string_view target,
        aul::Span<const std::string_view> dictionary,
        similarity_metric sim_func,
        std::size_t max_distance,
        std::size_t n
    ) {
        // Compute distances for each string.
        std::vector<Rank_string_view_pair> results;
        results.resize(dictionary.size());

        std::size_t threshold = max_distance + (max_distance != SIZE_MAX);

        std::size_t occupancy = 0;
        for (const auto& target_word : dictionary) {
            auto cost = sim_func(target, target_word, threshold);

            if (cost <= max_distance) {
                results[occupancy].distance = cost;
                results[occupancy].view = target_word;
                ++occupancy;
            }
        }
        results.resize(occupancy);

        // Construct vector containing results
        std::vector<std::string_view> ret;
        ret.resize(n);

        for (std::size_t i = 0; i < std::min(results.size(), n); ++i) {
            ret[i] = results[i].view;
        }

        return ret;
    }

}
