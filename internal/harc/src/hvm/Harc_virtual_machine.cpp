#include <harc/hvm/Harc_virtual_machine.hpp>

namespace harc::hvm {

    std::vector<Object> run_function(
        const ir::Function& function,
        std::vector<Object> arguments
    ) {
        //TODO: Check if function is pure

        //TODO: Check if object types match called function

        //TODO: Check if number of arguments matches arguments of function


    }

}
