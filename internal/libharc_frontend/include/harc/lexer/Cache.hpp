#ifndef HARC_LEX_CACHE_HPP
#define HARC_LEX_CACHE_HPP

#include <string_view>
#include <vector>
#include <array>
#include <cstdint>
#include <span>
#include <string>

#include <harc/lexer/Lexer.hpp>

namespace harc::lex {

    struct Cache_entry {
        std::string path;
        std::uint64_t time_stamp;

        Tokenization tokenization;
    };

    struct Cache {

        std::vector<Cache_entry> entries;

    };

    ///
    /// \param bytes
    /// \return
    [[nodiscard]]
    Cache parse_binary_cache(std::span<const std::byte> bytes);

    ///
    /// \param cache
    /// \return
    [[nodiscard]]
    Cache parse_text_cache(std::string_view cache);

}

#endif //HARC_LEX_CACHE_HPP
