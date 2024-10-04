#ifndef HARC_TOKENIZER_HPP
#define HARC_TOKENIZER_HPP

//#include <harc/Errors.hpp>
//#include <harc/Translation_unit.hpp>

#include <harc/lexer/Tokens.hpp>
#include <harc/lexer/Lexing_errors.hpp>

#include <harc/Message_buffer.hpp>

#include <string_view>
#include <vector>
#include <cstdint>
#include <span>

#include <aul/Math.hpp>

namespace harc::lex {

    struct Config {

        ///
        /// Maximum number of tokenization errors to report
        ///
        std::uint32_t max_errors = 100;

    };

    ///
    /// Represents a position within a source file
    ///
    struct Position {
        std::uint32_t line = 0;
        std::uint32_t column = 0;
    };

    ///
    /// Struct representing the tokenize of a Harmonia source file.
    ///
    /// TODO: Utilize a different data structure
    ///
    struct Tokenization {

        ///
        /// View over UTF-8 encoded Harmonia source code.
        ///
        /// A simple copy over the source parameter of the lex function.
        ///
        std::string_view source{};

        ///
        /// List of token types encountered during lexing
        ///
        std::vector<Token_type> types{};

        ///
        /// Byte indices of each token
        ///
        std::vector<std::uint32_t> source_indices{};

        ///
        /// Length, in bytes, of each token
        ///
        std::vector<std::uint32_t> lengths{};

        ///
        /// Byte index at which each line begins
        ///
        std::vector<std::uint32_t> line_indices;

        ///
        /// Index of token which pairs up with the current token. For tokens
        /// which are not paired, this will be the token's own index.
        ///
        std::vector<std::uint32_t> pair_indices;

        ///
        /// Contains messages produced by tokenization process.
        ///
        Message_buffer message_buffer;

        ///
        /// Indicates if the tokenization process completed without
        /// encountering an error. The message buffer may still contain
        /// warnings.
        ///
        bool success = false;

    };

    ///
    /// \param source_id A string that identifies the source file. Ideally, this
    /// should be absolute path of a source file when relevant. Otherwise, such
    /// as when the source is from memory, a unique name should suffice.
    /// \param source View over UTF-8 encoded Harmonia source code
    /// \param config Reference to Lexer configuration
    /// \return Tokenization object containing tokenization results
    [[nodiscard]]
    Tokenization lex(
        std::string_view source_id,
        std::string_view source,
        const Config& config = {}
    );

    ///
    ///
    ///
    /// \param tokenization Arbitrary tokenization object
    /// \return String representation of tokenization
    [[nodiscard]]
    std::string to_string(const Tokenization& tokenization);

    //[[nodiscard]]
    //bool requires_utf8(Translation_unit& unit);

    ///
    ///
    ///
    ///
    /// \param unit Translation unit to parse source for
    //Error_code lex(Translation_unit& unit);

}

#endif //HARC_TOKENIZER_HPP
