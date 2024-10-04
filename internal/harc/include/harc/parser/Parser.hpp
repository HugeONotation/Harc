#ifndef HARC_PARSER_HPP
#define HARC_PARSER_HPP

#include <harc/Translation_unit.hpp>
#include <harc/Errors.hpp>

namespace harc::parser {

    ///
    /// \param unit Translation unit to parse.
    /// \return
    Error_code parse(Translation_unit& unit);

}

#endif //HARC_PARSER_HPP
