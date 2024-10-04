#include "../include/harc/Error_reporting.hpp"


namespace harc {

    void print_error(
        std::string_view message_string
    ) {
        fmt::print(fmt::fg(fmt::color::crimson) | fmt::emphasis::bold, "Error: ");
        fmt::print(fmt::fg(fmt::color::crimson), "{}\n\n", message_string);
    }

    void print_error(
        std::string_view path,
        std::string_view message
    ) {
        fmt::print(fmt::fg(fmt::color::crimson) | fmt::emphasis::bold, "Error:");
        fmt::print(fmt::fg(fmt::color::crimson), " {}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "{}\n", path);
    }

    void print_error(
        std::string_view path,
        std::string_view source,
        std::string_view message
    ) {
        fmt::print(fmt::fg(fmt::color::crimson) | fmt::emphasis::bold, "Error:");
        fmt::print(fmt::fg(fmt::color::crimson), " {}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "{}\n", path);
        fmt::print("{}\n\n", source);
    }

    void print_error(
        std::string_view path,
        std::string_view source,
        std::array<std::uint32_t, 2> location,
        std::string_view message
    ) {
        fmt::print(fmt::fg(fmt::color::crimson) | fmt::emphasis::bold, "Error:");
        fmt::print(fmt::fg(fmt::color::crimson), " {}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "Line {}, column {} in {}\n", location[0], location[1], path);
        fmt::print("{}\n\n", source);
    }

    void print_warning(
        std::string_view message,
        std::string_view path,
        std::array<std::uint32_t, 2> location,
        std::string_view source
    ) {
        fmt::print(fmt::fg(fmt::color::orange) | fmt::emphasis::bold, "Warning: ");
        fmt::print(fmt::fg(fmt::color::orange), "{}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "Line {}, column {} in {}\n", location[0], location[1], path);
        fmt::print("{}\n\n", source);
    }

    void print_info(
        std::string_view message
    ) {
        fmt::print(fmt::fg(fmt::color::deep_sky_blue) | fmt::emphasis::bold, "Info: ");
        fmt::print(fmt::fg(fmt::color::deep_sky_blue), "{}\n", message);
    }

    void print_info(
        std::string_view message,
        std::string_view path,
        std::string_view source
    ) {
        fmt::print(fmt::fg(fmt::color::deep_sky_blue) | fmt::emphasis::bold, "Info: ");
        fmt::print(fmt::fg(fmt::color::deep_sky_blue), "{}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "{}\n", path);
        fmt::print("{}\n\n", source);
    }

    void print_info(
        std::string_view message,
        std::string_view path,
        std::array<std::uint32_t, 2> location,
        std::string_view source
    ) {
        fmt::print(fmt::fg(fmt::color::deep_sky_blue) | fmt::emphasis::bold, "Info: ");
        fmt::print(fmt::fg(fmt::color::deep_sky_blue), "{}\n", message);
        fmt::print(fmt::fg(fmt::color::cadet_blue), "Line {}, column {} in {}\n", location[0], location[1], path);
        fmt::print("{}\n\n", source);
    }

}
