#ifndef HARC_CLIENTS_HPP
#define HARC_CLIENTS_HPP

#include <string_view>
#include <cstdint>

namespace harc {



    struct Device {

        ///
        /// \return Name of compilation device
        [[nodiscard]]
        std::string_view name() const;

        ///
        ///
        /// \return
        std::uint64_t latency_estimate() const;

    };

    struct Client {



    };

}

#endif //HARC_CLIENTS_HPP
