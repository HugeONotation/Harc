#ifndef HARMONIA_TOKENIZATION_CUDA_HPP
#define HARMONIA_TOKENIZATION_CUDA_HPP

#include <harc/lexer/Tokens.hpp>

#include <string_view>

namespace harc::cuda {

    struct Tokenization {
        Token_type* type;

        std::string_view source_path;

        std::uint32_t* source_indices;

        std::uint32_t* lengths;

        std::array<std::uint32_t, 2>* source_locations;

        std::uint32_t* line_indices;

        std::uint32_t* pair_indices;

        std::uint32_t token_count = 0;
    };

}

#endif //HARMONIA_TOKENIZATION_CUDA_HPP
