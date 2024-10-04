#include <harc/lexer_cuda/Lexer_cuda.hpp>
#include <harc/lexer/Tokenization.hpp>
#include <harc/lexer/Lexing_errors.hpp>
#include <harc/cuda_common/Codepoint_manipulation.cuh>

#include <cstdint>
#include <harc/Translation_unit_cuda.hpp>

namespace harc::cuda::tokenizer {

    const std::uint32_t no_codepoint = 0xffffffffu;
    const std::uint32_t invalid_codepoint = 0xfffffffeu;



    ///
    /// \param byte_ptr Pointer to 4 sequential bytes which may be part of a
    /// codepoint beginning at the specified address.
    /// \return Decoded Unicode codepoint. no_codepoint if byte_ptr points to a
    /// continuation byte. invalid_codepoint if encoding is invalid.
    __device__
    std::uint32_t decode_utf8_codepoint(const char* byte_ptr) {
        // Read all bytes which may potentially be part of a codepoint beginning
        // at byte_ptr
        std::uint32_t codepoint_bytes;
        memcpy(&codepoint_bytes, byte_ptr, 4);

        std::uint32_t first_byte = codepoint_bytes & 0xff;

        // Return byte if the leading bit isn't set
        if (first_byte < 128) {
            return first_byte;
        }

        std::uint32_t leading_ones_count = __clz(~first_byte);

        // Set ret to  -1 if the current byte is a continuation byte
        std::uint32_t codepoint = 0;
        if (leading_ones_count == 1) {
            codepoint = UINT32_MAX;
        }

        // Making for zeroing out any bytes which aren't occupied by the codepoint
        // beginning with this thread's bytes
        std::uint32_t occupied_bytes_mask = ((std::uint32_t(1) << (leading_ones_count * 8)) - 1) << 8;

        // Zero out any bytes which are occupied by other codepoints
        std::uint32_t codepoint_bits = codepoint_bytes & occupied_bytes_mask;

        std::uint32_t leading_byte_mask = (1 << (7 - leading_ones_count)) - 1;

        // TODO: Consider use of 8/16-bit dot product instructions. This could
        //  potentially be an alternative that shaves off a few instructions
        codepoint |= codepoint_bits & leading_byte_mask;
        codepoint |= (codepoint_bits >> 0x08) & 0x3f;
        codepoint |= (codepoint_bits >> 0x10) & 0x3f;
        codepoint |= (codepoint_bits >> 0x18) & 0x3f;

        if (codepoint > 0x10FFFF) {
            codepoint = invalid_codepoint;
        }

        // Check that shortest form is respected
        if (leading_ones_count == 2 && codepoint <= 0x80) {
            codepoint = invalid_codepoint;
        }

        if (leading_ones_count == 3 && codepoint <= 0x800) {
            codepoint = invalid_codepoint;
        }

        if (leading_ones_count == 4 && codepoint <= 0x10000) {
            codepoint = invalid_codepoint;
        }

        bool is_surrogate = 0xD800 <= codepoint && codepoint <= 0xDBFF;
        if (is_surrogate) {
            codepoint = invalid_codepoint;
        }

        //Mask containing the bits that would come from
        const std::uint32_t continuation_byte_indicators = 0xC0C0C0C0u;

        //Check if there are four consecutive continuation bytes, which would be invalid
        if ((codepoint_bytes & continuation_byte_indicators) == continuation_byte_indicators) {
            codepoint = invalid_codepoint;
        }

        return codepoint;
    }

    ///
    /// Decodes a buffer of utf-8 encoded text into Unicode codepoints
    ///
    /// \param source Pointer to buffer containing raw utf-8 encoded source code.
    /// \param codepoint_buffer Buffer into which codepoints will be placed.
    __global__
    void decode_codepoints(const char* source, std::uint32_t* codepoint_buffer) {
        std::size_t index = blockIdx.x * blockDim.x + threadIdx.x;

        codepoint_buffer[index] = decode_utf8_codepoint(source + index);
    }

    ///
    ///
    /// \param x First codepoint
    /// \param y Second codepoint
    /// \param z Third codepoint
    /// \return Type of token
    __device__
    harc::Token_type identify_token(std::uint32_t x, std::uint32_t y, std::uint32_t z) {
        harc::Token_type ret = Token_type::NULL_TOKEN;
        if (x < 128) {

        } else {

        }

        return harc::Token_type::NULL_TOKEN;
    }

    __global__
    void tokenization_kernel_ascii(
        char* source_bytes,
        lex::Error* error_buffer,
        std::size_t error_buffer_length
    ) {
        //
    }

    __global__
    void tokenization_kernel_utf8(
        char* source_bytes,
        std::uint32_t* codepoint_buffer,
        lex::Error* error_buffer,
        std::size_t error_buffer_length
    ) {

    }

    ///
    /// \param code_units. Pointer to buffer containing utf8 code units
    /// \param requires_utf8 Output variable. 1 if the text uses codepoints greater than 127. 0 Otherwise
    __global__
    void requires_utf8_kernel(char* code_units, int* requires_utf8) {
        std::size_t index = blockIdx.x * blockDim.x + threadIdx.x;

        std::int8_t byte = code_units[index];

        // Check if leading bit is set
        if (byte < 0) {
            *requires_utf8 = 1;
        }
    }

    void lex_ascii(
        dim3 grid,
        dim3 block,
        cudaStream_t stream,
        Translation_unit& unit,
        std::uint32_t warp_size
    ) {
        cudaEvent_t kernel_start;
        cudaEvent_t kernel_stop;

        cudaEventCreate(&kernel_start);
        cudaEventCreate(&kernel_stop);

        // Allocate error buffer
        cudaMallocAsync(&unit.device_error_buffer, unit.error_buffer_size, stream);


    }

    void lex_utf8(
        dim3 grid,
        dim3 block,
        cudaStream_t stream,
        Translation_unit& unit,
        std::uint32_t warp_size
    ) {

        cudaEvent_t kernel_start;
        cudaEvent_t kernel_stop;

        cudaEventCreate(&kernel_start);
        cudaEventCreate(&kernel_stop);

        // Allocate codepoint buffer
        cudaMallocAsync(
            &unit.device_codepoint_buffer,
            unit.codepoint_buffer_size,
            stream
        );

        cudaMemsetAsync(
            unit.device_codepoint_buffer,
            0,
            unit.codepoint_buffer_size,
            stream
        );

        cudaStreamSynchronize(stream);

        cudaError_t err = cudaError_t::cudaSuccess;

        // Dispatch utf8 tokenization kernel
        err = cudaEventRecord(kernel_start, stream);
        decode_codepoints<<<grid, block, 0, stream>>>(
            unit.device_source_buffer,
            unit.device_codepoint_buffer
        );
        err = cudaEventRecord(kernel_stop, stream);

        cudaStreamSynchronize(stream);

        err = cudaEventElapsedTime(
            &unit.timings.tokenization_latency,
            kernel_start,
            kernel_stop
        );

        cudaEventDestroy(kernel_start);
        cudaEventDestroy(kernel_stop);

        cudaStreamSynchronize(stream);
    }

    void lex(
        dim3 grid,
        dim3 block,
        cudaStream_t stream,
        Translation_unit& unit,
        std::uint32_t warp_size
    ) {
        dim3 grid_size{};
        dim3 block_size{};

        if (unit.is_pure_ascii) {
            lex_ascii(grid, block, stream, unit, warp_size);
        } else {
            lex_utf8(grid, block, stream, unit, warp_size);
        }
    }

}
