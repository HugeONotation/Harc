#ifndef LIBHARC_CORE_OS_UTILS_HPP
#define LIBHARC_CORE_OS_UTILS_HPP

#include <span>
#include <string>
#include <string_view>

//#include <harc/Errors.hpp>

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
    Process_output run(std::string command, std::span<char*> arguments);



    ///
    /// Memory map a file to be interpreted as a sequence of bytes.
    ///
    /// \param view View over null-terminated string containing path to file
    /// \return
    [[nodiscard]]
    std::span<const std::byte> map_file(std::string_view path);

    ///
    /// \param bytes Span over bytes that was returned by an earlier call to
    ///  map_binary_file
    void unmap_file(std::span<const std::byte> bytes);

}

#endif //LIBHARC_CORE_OS_UTILS_HPP
