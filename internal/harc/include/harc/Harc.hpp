#ifndef HARMONIA_HARC_HPP
#define HARMONIA_HARC_HPP

#include <harc/Version.hpp>
#include <harc/Errors.hpp>

#include <harc/Config.hpp>

#include <span>
#include <string_view>

namespace harc {

    //=====================================================
    // Versioning
    //=====================================================

    // Harc's version
    constexpr Version version{0, 0, 0, Release_type::ALPHA};

    ///
    /// Run Harc as a client.
    /// Hard will receive work from a host instance.
    ///
    /// \param config Harc compiler client configuration
    void run_client(
        const Config& config = {}
    );

    ///
    /// Run Harc as a server
    /// This instance of Harc will not perform compilation tasks
    ///
    /// \param source_paths Paths to files to compile.
    /// \param config Harc compiler server configuration
    void run_pure_server(
        std::span<const std::string_view> source_paths,
        const Config& config = {}
    );

    ///
    /// Run Harc as a server.
    /// This instance of Harc will also perform compilation tasks.
    ///
    /// \param source_paths Paths to files to compile.
    /// \param config Harc compiler server configuration
    void run_server(
        std::span<const std::string_view> source_paths,
        const Config& config = {}
    );

    ///
    /// Run Harc entirely locally.
    ///
    /// \param source_paths Paths to files to compile.
    /// \param config Harc compiler server configuration
    void run_locally(
        std::span<const std::string_view> source_paths,
        const Config& config = {}
    );

}

#endif //HARMONIA_HARC_HPP
