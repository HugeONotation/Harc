#ifndef HARMONIA_TRANSLATION_UNIT_CUDA_HPP
#define HARMONIA_TRANSLATION_UNIT_CUDA_HPP

#include <harc/lexer/Tokenization.hpp>
#include <harc/lexer/Lexing_errors.hpp>

#include <harc/Errors.hpp>

#include <aul/Span.hpp>

#include <cstdint>
#include <string_view>

namespace harc::cuda {

    constexpr std::size_t max_cuda_block_size = 1024;

    ///
    /// Maximum number of errors that will be reported during tokenization state
    ///
    constexpr std::size_t max_tokenization_errors = 128;

    ///
    /// Maximum number of errors that will be reported during parsing state
    ///
    constexpr std::size_t max_parsing_errors = 128;

    ///
    /// Maximum number of errors that will be reported during semantic analysis
    ///
    constexpr std::size_t max_semantic_analysis_errors = 128;



    ///
    /// Structure for reporting timing information for various stages of the
    /// compiler. All measurements are in units of .
    ///
    struct Timing_information {
        float required_utf8_latency = NAN;
        float utf8_decode_latency = NAN;
        float tokenization_latency = NAN;
    };



    ///
    /// Maintains all state relevant for processing and an individual
    /// translation unit.
    ///
    struct Translation_unit {

        //=================================================
        // -ctors
        //=================================================

        explicit Translation_unit(std::string_view source_path);
        ~Translation_unit();

        //=================================================
        // Instance members
        //=================================================

        ///
        /// View for path to source file
        ///
        std::string_view source_path{};

        ///
        /// Pointer to memory-mapped contents of source file contents
        ///
        aul::Span<const char> source_mapping;

        ///
        ///
        ///
        bool is_pure_ascii = false;

        ///
        /// Size of source file in bytes
        ///
        std::uint32_t source_file_size;

        ///
        /// Pointer to device memory containing the contents of the source file
        ///
        char* device_source_buffer = nullptr;

        ///
        /// Pointer to device memory containing the contents of the source
        ///
        std::size_t source_buffer_size = 0;

        ///
        /// Pointer to buffer which is used for storing decoded codepoints
        ///
        std::uint32_t* device_codepoint_buffer = nullptr;

        ///
        /// Size of buffer used by
        ///
        std::size_t codepoint_buffer_size = 0;

        ///
        /// Pointer to buffer containing errors
        ///
        lex::Error_code* device_error_buffer = nullptr;

        ///
        ///
        ///
        std::size_t error_buffer_size = 0;

        ///
        ///
        ///
        Timing_information timings;
    };

    ///
    /// \param unit Unit to compute buffer sizes for
    /// \return Size of bytes that would be required to complete the
    /// tokenization phase
    inline void compute_tokenization_buffer_sizes(Translation_unit& unit) {
        // Add (max_cuda_block_size - 1) to perform division with upwards rounding
        // Add one to ensure enough space for a null terminator is available
        // Add four to ensure that the last thread can unconditionally read 4 bytes
        unit.source_buffer_size =
            (unit.source_file_size + (max_cuda_block_size - 1) + 1 + 3) /
            max_cuda_block_size *
            max_cuda_block_size;

        unit.codepoint_buffer_size = unit.source_buffer_size * sizeof(std::uint32_t);

        unit.error_buffer_size = max_tokenization_errors * sizeof(lex::Error);
    }

}

#endif //HARMONIA_TRANSLATION_UNIT_CUDA_HPP
