#ifndef HARC_OUTPUT_HPP
#define HARC_OUTPUT_HPP

#include <array>
#include <string_view>

#include <fmt/core.h>
#include <fmt/color.h>

namespace harc {

    //=====================================================
    // Error reporting functions
    //=====================================================

    void print_error(
        std::string_view message_string
    );

    void print_error(
        std::string_view path,
        std::string_view message
    );

    void print_error(
        std::string_view path,
        std::string_view source,
        std::string_view message
    );

    void print_error(
        std::string_view path,
        std::string_view source,
        std::array<std::uint32_t, 2> location,
        std::string_view message
    );

    //=====================================================
    // Warning reporting functions
    //=====================================================

    void print_warning(
        std::string_view message
    );

    void print_warning(
        std::string_view path,
        std::string_view source,
        std::string_view message
    );

    void print_warning(
        std::string_view path,
        std::string_view message,
        std::array<std::uint32_t, 2> location,
        std::string_view source
    );

    //=====================================================
    // Info reporting functions
    //=====================================================

    void print_info(
        std::string_view message
    );

    void print_info(
        std::string_view path,
        std::string_view source,
        std::string_view message
    );

    void print_info(
        std::string_view path,
        std::array<std::uint32_t, 2> location,
        std::string_view source,
        std::string_view message
    );

}

#endif //HARC_OUTPUT_HPP
