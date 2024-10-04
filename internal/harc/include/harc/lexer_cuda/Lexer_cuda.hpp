#ifndef HARMONIA_LEXER_CUDA_HPP
#define HARMONIA_LEXER_CUDA_HPP

#include "harc/Errors.hpp"
#include "harc/Translation_unit_cuda.hpp"

#include <cuda_runtime.h>

namespace harc::cuda {

    ///
    /// Launch tokenization kernel
    ///
    /// \param grid
    /// \param block
    /// \param stream
    /// \param unit
    void lex(
        dim3 grid,
        dim3 block,
        cudaStream_t stream,
        Translation_unit& unit,
        std::uint32_t warp_size
    );

}

#endif //HARMONIA_LEXER_CUDA_HPP
