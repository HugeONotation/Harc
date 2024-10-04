#ifndef HARC_PARSE_TREE_FWD_HPP
#define HARC_PARSE_TREE_FWD_HPP

namespace harc::parser {

    struct Parse_tree_node;

    struct Text;

    struct Resolved_identifier;

    struct Identifier;

    struct Expression;

    struct Expression_list;

    struct Expression_sequence;

    struct Type_expression;

    struct Resolved_template;

    struct Numeric_literal_expression;

    struct String_literal_expression;

    struct Codepoint_literal_expression;

    struct Tuple_literal;

    struct Parenthesized_expression;

    struct Unary_operator_expression;

    struct Binary_operator_expression;

    struct Binary_text_operator_expression;

    struct Function_call;

    struct Statement;

    struct Return_statement;

    struct Variable_declaration;

    struct Constant_declaration;

    struct Module_declaration;

    struct Attribute;

    struct Attribute_list_body;

    struct Attribute_list;

    struct Statement_sequence;

    struct Code_body;

    struct Parameter;

    struct Parameter_sequence;

    struct Parameter_list;

    struct Source_body;

    struct Function_definition;

    struct Lambda;

    struct Struct_definition;

    struct Alias_definition;

    struct Source_body_list;

    struct Source_file;

}

#endif //HARC_PARSE_TREE_FWD_HPP
