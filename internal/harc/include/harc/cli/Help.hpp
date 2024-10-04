#ifndef HARC_HELP_HPP
#define HARC_HELP_HPP

#include <string_view>

#include <aul/Span.hpp>
#include <array>

namespace harc::cli {

    ///
    /// Returns a std::string_view object over the contents of the help page.
    ///
    /// The returned std::string_view is empty when the passed in page_name
    /// does not name a help page.
    ///
    /// The default .
    ///
    /// \param page_name Unique identifying name of help page
    /// \return View over contents of requested help page
    [[nodiscard]]
    std::string_view query_help_page(std::string_view page_name);

    ///
    /// \return
    [[nodiscard]]
    aul::Span<const std::string_view> query_help_topics();

    //TODO: Add function to suggest a help page when the queried help page name
    // does not exist
    [[nodiscard]]
    std::array<std::string_view, 3> query_help_page_suggestions(
        std::string_view page_name
    );

}

#endif //HARC_HELP_HPP
