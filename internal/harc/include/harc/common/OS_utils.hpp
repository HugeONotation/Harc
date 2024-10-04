#ifndef HARC_OS_UTILS_HPP
#define HARC_OS_UTILS_HPP

#include <string>
#include <string_view>

#include <aul/Span.hpp>

#include <harc/Errors.hpp>

namespace harc {

    ///
    /// Utility struct used as the output of the run subroutine.
    ///
    struct Process_output {
        std::uint32_t status_code = 0;
        std::string std_out;
    };

    ///
    /// Invoke an external program.
    ///
    /// \param command Command to run
    /// \param arguments Arguments to pass to command
    /// \return Struct containing status code and stdout of command that was run
    [[nodiscard]]
    Process_output run(std::string command, aul::Span<char*> arguments);



    ///
    /// Utility struct used as the output of the map_binary_file subroutine.
    ///
    struct Binary_file_mapping {
        aul::Span<const std::byte> bytes{};
        Error_code error_code = Error_code::NO_ERROR;
    };

    ///
    /// Memory map a file to be interpreted as a sequence of bytes.
    ///
    /// \param view View over null-terminated string containing path to file
    /// \return
    [[nodiscard]]
    Binary_file_mapping map_binary_file(std::string_view path);

    ///
    /// \param bytes Span over bytes that was returned by an earlier call to
    ///  map_binary_file
    void unmap_file(aul::Span<std::byte> bytes);

    ///
    /// \param mapping Span over bytes that was returned by an earlier call to
    ///  map_binary_file
    void unmap_file(Binary_file_mapping mapping);



    ///
    /// Utility struct used as the output of the map_binary_file subroutine.
    ///
    struct Text_file_mapping {
        std::string_view text;
        Error_code error_code = Error_code::NO_ERROR;
    };

    ///
    /// Memory map a file to be interpreted as containing ASCII/UTF-8 text
    ///
    /// \param view View over null-terminated string containing path to file
    /// \return
    [[nodiscard]]
    Text_file_mapping map_text_file(std::string_view path);

    ///
    /// \param bytes
    void unmap_file(std::string_view mapping);

    ///
    /// \param mapping
    void unmap_file(Text_file_mapping mapping);

}

#endif //HARC_OS_UTILS_HPP
