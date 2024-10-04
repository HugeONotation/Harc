#ifndef LIBHARC_CORE_STRINGS_HPP
#define LIBHARC_CORE_STRINGS_HPP

#include <vector>
#include <string>
#include <string_view>
#include <span>

namespace harc {

    ///
    /// Split the string into a list of substring using the specified delimiter.
    ///
    /// \param str String to split into multiple substrings
    /// \param delimiter String that will mark the boundary between consecutive
    /// substrings
    /// \return List of views over the results substrings
    [[nodiscard]]
    std::vector<std::string_view> split(
        std::string_view str,
        std::string_view delimiter
    );


    ///
    /// Split the string into a list of substring using the specified delimiter.
    ///
    /// \param str String to split into multiple substrings
    /// \param delimiter Character that will mark the boundary between
    /// consecutive substrings
    /// \return List of views over the results substrings
    [[nodiscard]]
    std::vector<std::string_view> split(
        std::string_view str,
        char delimiter
    );

    ///
    /// Join the specified strings with the specified delimiter placed between.
    ///
    /// \param strings Span of strings to join together
    /// \param delimiter Delimiter tobe paced between consecutive joined strings
    /// \return
    [[nodiscard]]
    std::string join(
        std::span<const std::string_view> strings,
        std::string_view delimiter = ""
    );

    ///
    /// Replace all instances of a specified substring with another string.
    ///
    /// \param str View over string to perform replacements on
    /// \param target Substring to search for
    /// \param replacement Substring to replace target with
    /// \return
    [[nodiscard]]
    std::string replace(
        std::string_view str,
        std::string_view target,
        std::string_view replacement
    );

    /*
    ///
    /// Replaces all instances of the following codepoints with their escape
    /// sequences.
    ///
    /// TODO:
    ///
    /// \param str A view over the string whose contents should be escaped
    /// \return
    [[nodiscard]]
    std::string escape(std::string_view str);

    ///
    /// Replaces the following escape sequences with their corresponding
    /// codepoints.
    ///
    /// TODO:
    ///
    /// \param str A view over a string
    /// \return
    [[nodiscard]]
    std::string unescape(std::string_view str);
    */

}

#endif //LIBHARC_CORE_STRINGS_HPP
