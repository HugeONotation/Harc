#ifndef HARMONIA_HARC_VIRTUAL_MACHINE_HPP
#define HARMONIA_HARC_VIRTUAL_MACHINE_HPP

#include <harc/ir/IR.hpp>

#include <vector>
#include <cstdint>

namespace harc::hvm {

    ///
    ///
    ///
    struct Object {

    };

    ///
    /// \param function Harmonia function to evaluate
    /// \param arguments List of parameters to function
    /// \return Values returned by the function
    [[nodiscard]]
    std::vector<Object> run_function(
        const ir::Function& function,
        std::vector<Object> arguments
    );

}

#endif //HARMONIA_HARC_VIRTUAL_MACHINE_HPP
