#ifndef HARMONIA_ALGORITHMS_HPP
#define HARMONIA_ALGORITHMS_HPP

#include <vector>
#include <string>
#include <string_view>

#include <aul/Span.hpp>

namespace harc {

    ///
    /// \param str0 ASCII-encoded string
    /// \param str1 ASCII-encoded string
    /// \param max Maximum edit distance to compute.
    /// \return The Levenshtein edit distance between str0 and str1
    [[nodiscard]]
    std::size_t levenstein_edit_distance_ascii(
        std::string_view str0,
        std::string_view str1,
        std::size_t max = SIZE_MAX
    );

    /*
    ///
    /// \param str0 ASCII-encoded string
    /// \param str1 ASCII-encoded string
    /// \param max Maximum edit distance to compute.
    /// \return The Levenshtein edit distance between str0 and str1
    [[nodiscard]]
    std::size_t levenstein_edit_distance_utf8(
        std::string_view str0,
        std::string_view str1,
        std::size_t max = SIZE_MAX
    );
    */

    ///
    /// \param str0 ASCII-encoded string
    /// \param str1 ASCII-encoded string
    /// \param max Maximum edit distance to compute.
    /// \return The Damerau-Levenshtein edit distance between str0 and str1
    [[nodiscard]]
    std::size_t damerau_levenstein_edit_distance_ascii(
        std::string_view str0,
        std::string_view str1,
        std::size_t max = SIZE_MAX
    );

    /*
    ///
    /// \param str0 UTF8-encoded string
    /// \param str1 UTF8-encoded string
    /// \param max Maximum edit distance to compute.
    /// \return The Damerau-Levenshtein edit distance between str0 and str1
    [[nodiscard]]
    std::size_t damerau_levenstein_edit_distance_utf8(
        std::string_view str0,
        std::string_view str1,
        std::size_t max = SIZE_MAX
    );
    */

    using similarity_metric = std::size_t (*)(std::string_view, std::string_view, std::size_t max);

    ///
    /// \param target String to look for similarity for.
    /// \param dictionary List of strings to look through for similar strings
    /// \param sim_func Function to use to to compute string similarity
    /// \param n Target number of similar strings to return.
    /// \return A list of strings which are similar to the target
    [[nodiscard]]
    std::vector<std::string_view> most_similar(
        std::string_view target,
        aul::Span<const std::string_view> dictionary,
        similarity_metric sim_func,
        std::size_t max_distance = SIZE_MAX,
        std::size_t n = 1
    );

}

#endif //HARMONIA_ALGORITHMS_HPP
