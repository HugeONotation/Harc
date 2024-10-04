#ifndef HARC_SYMBOLS_HPP
#define HARC_SYMBOLS_HPP

#include <aul/containers/Array_map.hpp>

#include <string_view>
#include <vector>

namespace harc::symbols {

    struct Type {

    };

    struct Variable {
        const Type* type;

        std::uint64_t implementation_hash;
    };

    struct Subroutine {
        const Type* return_type;

        std::uint64_t implementation_hash;
    };

    struct Structure {
        aul::Array_map<std::string_view, Variable> public_variables;
        aul::Array_map<std::string_view, Variable> private_variables;

        aul::Array_map<std::string_view, Subroutine> public_subroutines;
        aul::Array_map<std::string_view, Subroutine> private_subroutines;
    };

    struct Symbol_table {

        ///
        ///
        ///
        Symbol_table* parent = nullptr;

        aul::Array_map<std::string_view, Variable> public_variables;
        aul::Array_map<std::string_view, Variable> private_variables;

        aul::Array_map<std::string_view, Subroutine> public_subroutines;
        aul::Array_map<std::string_view, Subroutine> private_subroutines;

        aul::Array_map<std::string_view, Variable> public_structures;
        aul::Array_map<std::string_view, Variable> private_structures;

        aul::Array_map<std::string_view, Type> public_types;
        aul::Array_map<std::string_view, Type> private_types;
    };

}

#endif //HARC_SYMBOLS_HPP
