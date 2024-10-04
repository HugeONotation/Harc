#ifndef HARC_PARSER_PARSER_HPP
#define HARC_PARSER_PARSER_HPP

#include <harc/lexer/Lexer.hpp>
#include <harc/parser/Parsing_errors.hpp>

#include <vector>

namespace harc::parser {

    struct Config {

        std::uint32_t error_buffer_size = 1000;

    };

    struct Parse_heap {

        std::vector<Error> errors;

    };

    [[nodiscard]]
    Parse_heap parse(const lex::Tokenization& tokenization, const Config& config = {});

}

#endif //HARC_PARSER_PARSER_HPP
