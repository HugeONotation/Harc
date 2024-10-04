#include <harc/common/OS_utils.hpp>

#include <string_view>
#include <span>

#include <harc/Error_reporting.hpp>
#include <harc/Errors.hpp>

#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace harc {

    Process_output run(std::string command, aul::Span<char*> arguments) {
        #if HARC_POSIX
        //TODO: Add timeout to child process

        if (0 == getpid()) {
            // TODO: Report warning!

            return Process_output{};
        }

        int file_descs[2];
        if (pipe(file_descs) == -1) {
            // TODO: Perform error handling
        }

        int& pipe_out = file_descs[0];
        int& pipe_in  = file_descs[1];

        pid_t child_pid = fork();
        int child_status;
        std::string child_std_out{};

        if (child_pid) {
            // Parent process handling
            waitpid(child_pid, &child_status, 0);

            char buffer[4 * 1024];
            while (true) {
                std::size_t count = read(pipe_out, buffer, sizeof(buffer));
                if (count == 0) {
                    break;
                }

                if (count == -1) {
                    //TODO: Perform error handling
                    break;
                }

                child_std_out += std::string_view{buffer, count};
            }

            close(pipe_in);
        } else {
            // Child process handling

            // Reroute output to pipe
            while ((dup2(pipe_in, STDOUT_FILENO) == -1) && (errno == EINTR)) {}

            // Close pipe ends
            close(pipe_out);
            close(pipe_in);

            // Execute command
            execv(command.data(), arguments.data());
        }

        // Populate return value
        Process_output output;
        output.std_out = child_std_out;
        output.status_code = child_status;

        return output;

        #else
        static_assert(
            false,
            "This function is not implemented for the target OS"
        );

        return {};
        #endif
    }

    Binary_file_mapping map_binary_file(std::string_view file_path) {
        Binary_file_mapping ret{};

        #if HARC_POSIX
        // Acquire file descriptor to file
        int fd = open(file_path.data(), O_RDONLY);

        if (fd == -1) {
            //TODO: Log error

            ret.error_code = Error_code::INACCESSIBLE_SOURCE_FILE_PATH;
            return ret;
        }

        // Query and validate source file length
        struct stat file_statistics{};
        stat(file_path.data(), &file_statistics);

        // Memory map file contents
        char* mapping_ptr = static_cast<char*>(mmap(
            nullptr,
            file_statistics.st_size,
            PROT_READ,
            MAP_PRIVATE,
            fd,
            0 // Offset
        ));

        if (mapping_ptr == MAP_FAILED) {
            //TODO: Log error

            ret.error_code = Error_code::GENERAL_INTERNAL_ERROR;
            return ret;
        }

        ret.bytes = aul::Span<const std::byte>{
            reinterpret_cast<std::byte*>(mapping_ptr),
            std::size_t(file_statistics.st_size)
        };

        return ret;

        #else
        static_assert(false, "This function is not implemented for the target OS");
        return {};

        #endif
    }

    void unmap_file(aul::Span<const std::byte> bytes) {
        if (!bytes.data()) {
            return;
        }

        #if defined(HARC_POSIX)
        munmap(const_cast<std::byte*>(bytes.data()), bytes.size());

        #else
        static_assert(false, "This function is not implemented for the target OS");

        #endif
    }

    void unmap_file(Binary_file_mapping mapping) {
        unmap_file(mapping.bytes);
    }

    Text_file_mapping map_text_file(std::string_view file_path) {
        auto result = map_binary_file(file_path);

        Text_file_mapping ret;
        ret.error_code = result.error_code;
        ret.text = std::string_view{
            reinterpret_cast<const char*>(result.bytes.data()),
            result.bytes.size()
        };
        return ret;
    }

    void unmap_file(std::string_view mapping) {
        unmap_file(aul::Span<const std::byte>{
            reinterpret_cast<const std::byte*>(mapping.data()),
            mapping.size()
        });
    }

    void unmap_file(Text_file_mapping mapping) {
        unmap_file(mapping.text);
    }

}
