#ifndef HARC_TRANSLATION_UNIT_HPP
#define HARC_TRANSLATION_UNIT_HPP

#include <harc/lexer/Lexer.hpp>
#include <harc/parser/Parse_tree.hpp>
#include <harc/Symbols.hpp>

#include "lexer_cuda/Lexing_cuda.hpp"

#include <string_view>

namespace harc {

    ///
    /// Enum used to indicate success status in attempt to process translation
    /// unit.
    ///
    /// Translation units will continue to be processed even after failing, to
    /// the extent possible. This is done in an attempt to provide the most
    /// useful error messages possible. However harc will not proceed beyond
    /// semantic analysis if any translation unit has had an error.
    ///
    enum class Translation_unit_status {
        NO_ERROR,
        TOKENIZATION_FAILED,
        PARSING_FAILED,
        SEMANTIC_ANALYSIS_FAILED
    };

    ///
    /// Struct which represents a cached representation of a translation unit.
    ///
    struct Translation_unit_cache {

        ///
        /// Number of bytes long that the source from which the cache was
        /// compiled.
        ///
        std::uint32_t source_length = 0;

        ///
        /// Hash of source file
        ///
        /// Computed via Murmur hash v3 x86-64
        ///
        std::array<std::uint32_t, 4> hash{};

    };

    ///
    /// This struct contains all the information which is stored pertinent to a
    /// particular source file.
    ///
    struct Translation_unit {

        Translation_unit() = default;

        Translation_unit(const Translation_unit&) = delete;
        Translation_unit(Translation_unit&&) noexcept = default;

        Translation_unit& operator=(const Translation_unit&) = delete;
        Translation_unit& operator=(Translation_unit&&) noexcept = default;

        //=================================================
        // Fields
        //=================================================

        ///
        /// View over string containing path from which source was read.
        ///
        std::string_view source_path;

        ///
        /// String containing contents of source
        ///
        std::string source;

        ///
        /// Token sequence and necessary metadata
        ///
        lex::Tokenization tokenization;

        ///
        /// Parse tree
        ///
        parser::Parse_tree parse_tree;

        ///
        /// Symbols defined within this translation unit
        ///
        std::vector<symbols::Symbol_table> symbol_tables;

        ///
        /// Error status of this unit
        ///
        Translation_unit_status status = Translation_unit_status::NO_ERROR;

        //=================================================
        // Accessors
        //=================================================

        ///
        /// \param token_index Index of token to get source
        /// \return View over substring of source which corresponds to the
        /// requested token
        [[nodiscard]]
        std::string_view token_source(std::uint32_t token_index) const {
            return std::string_view{
                source.data() + tokenization.source_indices[token_index],
                tokenization.lengths[token_index]
            };
        }

    };

    ///
    /// \param unit Translation unit whose tokenization should be printed
    /// \return String representation of translation unit's tokenization
    [[nodiscard]]
    std::string tokenization_to_string(const Translation_unit& unit);

    [[nodiscard]]
    std::string_view excerpt_with_context(
        const Translation_unit& unit,
        std::array<std::uint32_t, 2> excerpt_location
    );

}

#endif //HARC_TRANSLATION_UNIT_HPP
