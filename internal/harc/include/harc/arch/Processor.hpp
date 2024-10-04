#ifndef HARMONIA_PROCESSOR_HPP
#define HARMONIA_PROCESSOR_HPP

#include "Uarch.hpp"

#include <vector>

namespace harc {

    struct Processor {
        Uarch* uarch = nullptr;
        std::uint32_t threads = 0;

    };

    struct CPU {
        std::vector<Processor> cores;

        [[nodiscard]]
        std::uint32_t logical_core_count() {
            std::uint32_t ret = 0;

            for (auto& c : cores) {
                ret += c.threads;
            }

            return ret;
        }

        [[nodiscard]]
        std::uint32_t core_count() {
            return cores.size();
        }

    };

}

#endif //HARMONIA_PROCESSOR_HPP
