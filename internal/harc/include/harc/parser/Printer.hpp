#ifndef HARC_PRINTER_HPP
#define HARC_PRINTER_HPP

#include <harc/Translation_unit.hpp>

namespace harc {

    [[nodiscard]]
    std::string parse_tree_to_string(const Translation_unit& unit);

}

#endif //HARC_PRINTER_HPP
