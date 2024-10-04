#ifndef HARMONIA_PARSE_TREE_VISITOR_HPP
#define HARMONIA_PARSE_TREE_VISITOR_HPP

#include "Parse_tree_fwd.hpp"

namespace harc::parser {

    class Visitor {
    public:

        //=================================================
        // Visit methods
        //=================================================

        virtual void visit(std::uint32_t token_index) = 0;

        virtual void visit(Parse_tree_node* node) = 0;

        virtual void visit(Text*) = 0;

        virtual void visit(Resolved_identifier* node) = 0;

        virtual void visit(Identifier* node) = 0;

        virtual void visit(Expression* node) = 0;

        virtual void visit(Expression_list* node) = 0;

        virtual void visit(Function_call* node) = 0;

        virtual void visit(Statement* node) = 0;

        virtual void visit(Return_statement* node) = 0;

        virtual void visit(Variable_declaration* node) = 0;

        virtual void visit(Constant_declaration* node) = 0;

        virtual void visit(Module_declaration* node) = 0;

        virtual void visit(Attribute* node) = 0;

        virtual void visit(Attribute_list_body* node) = 0;

        virtual void visit(Attribute_list* node) = 0;

        virtual void visit(Statement_sequence* node) = 0;

        virtual void visit(Code_body* node) = 0;

        virtual void visit(Parameter* node) = 0;

        virtual void visit(Parameter_sequence* node) = 0;

        virtual void visit(Parameter_list* node) = 0;

        virtual void visit(Source_body* node) = 0;

        virtual void visit(Function_definition* node) = 0;

        virtual void visit(Lambda* node) = 0;

        virtual void visit(Struct_definition* node) = 0;

        virtual void visit(Alias_definition* node) = 0;

        virtual void visit(Source_body_list* node) = 0;

        virtual void visit(Source_file* node) = 0;

    };

}

#endif //HARMONIA_PARSE_TREE_VISITOR_HPP
