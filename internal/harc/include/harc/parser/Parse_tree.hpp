#ifndef HARC_PARSE_TREE_HPP
#define HARC_PARSE_TREE_HPP

#include "Parse_tree_visitor.hpp"

namespace harc::parser {

    enum class Node_type {
        GENERIC_NODE,
        TEXT,
        RESOLVED_IDENTIFIER,
        IDENTIFIER,
        STATEMENT,
        EXPRESSION,
        EXPRESSION_LIST,
        EXPRESSION_SEQUENCE,
        TYPE_EXPRESSION,
        FUNCTION_CALL,
        RETURN_STATEMENT,
        VARIABLE_DECLARATION,
        CONSTANT_DECLARATION,
        MODULE_DECLARATION,
        ATTRIBUTE,
        ATTRIBUTE_LIST,
        CODE_BODY,
        PARAMETER,
        PARAMETER_LIST,
        SOURCE_BODY,
        FUNCTION_DECLARATION,
        LAMBDA,
        CLASS_DEFINITION,
        ALIAS_DEFINITION,
        SOURCE_BODY_LIST,
        SOURCE_FILE
    };

    struct Parse_tree_node {
        std::int32_t index = -1;

        virtual void accept(Visitor& visitor) {
            visitor.visit(this);
        }
    };

    struct Text : Parse_tree_node {
        std::uint32_t text_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Text_sequence : Parse_tree_node {
        Text* head = nullptr;
        std::uint32_t comma_token = -1;
        Text_sequence* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Resolved_identifier : Parse_tree_node {
        Text* head = nullptr;
        std::uint32_t dot_token = -1;
        Resolved_identifier* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Identifier : Resolved_identifier {
        Text* name = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Statement : Parse_tree_node {

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }

    };

    struct If_statement : Statement {
        Text* if_keyword = nullptr;
        Expression* condition = nullptr;
        Code_body* true_branch = nullptr;

        std::vector<Text*> else_keywords{};
        std::vector<Text*> else_if_keywords{};
        std::vector<Code_body*> else_if_branches{};

        Text* else_keyword = nullptr;
        Code_body* else_branch = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct While_statement : Statement {
        Text* while_keyword = nullptr;
        Code_body* loop_body = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct For_loop : Statement {
        Text* for_keyword = nullptr;
        Statement_sequence* initializers = nullptr;
        Expression* condition = nullptr;
        Statement_sequence* incrementer = nullptr;
        Code_body* loop_body = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Ranged_for_loop : Statement {
        Text* for_keyword = nullptr;
        Text_sequence* loop_variables = nullptr;
        Text* in_keyword = nullptr;
        Expression_sequence* ranges = nullptr;
        Code_body* loop_body = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Do_while_loop : Statement {
        Text* do_keyword = nullptr;
        Code_body* code_body = nullptr;
        Text* while_keyword = nullptr;
        Expression* condition = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    /*
    struct Switch_statement :  Statement {


        virtual void accept(Visitor& visitor) {
            visitor.visit(this);
        }
    };
    */

    struct Expression : Statement {

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Expression_list : Parse_tree_node {
        Expression* head = nullptr;
        std::uint32_t semicolon_token = -1;
        Expression_list* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Expression_sequence : Parse_tree_node {
        Expression* head = nullptr;
        std::uint32_t comma_token = -1;
        Expression_sequence* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Type_expression : Expression {

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }

    };

    struct Resolved_template : Expression {
        Type_expression* template_expression = nullptr;
        std::uint32_t l_template_bracket = -1;
        Expression_sequence* parameters = nullptr;
        std::uint32_t r_template_bracket = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Resolved_type : Type_expression {
        Type_expression* tail = nullptr;
        std::uint32_t period_token = -1;
        Text* head = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Numeric_literal_expression : Expression {
        std::uint32_t numeric_literal_token = -1;
        std::uint32_t colon_token = -1;
        Expression* type_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct String_literal_expression : Expression {
        std::uint32_t string_literal_token = -1;
        std::uint32_t colon_token = -1;
        Expression* type_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Codepoint_literal_expression : Expression {
        std::uint32_t codepoint_literal_token = -1;
        std::uint32_t colon_token = -1;
        Expression* type_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Tuple_literal : Expression {
        std::uint32_t l_curly_bracket_token = -1;
        Expression_sequence* expression_sequence = nullptr;
        std::uint32_t r_curly_bracket_token = -1;
        std::uint32_t colon_token = -1;
        Type_expression* type_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Parenthesized_expression : Expression {
        std::uint32_t l_paren_token = -1;
        Expression* expression = nullptr;
        std::uint32_t r_paren_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Unary_operator_expression : Expression {
        std::uint32_t operator_token = -1;
        Expression* expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Binary_operator_expression : Expression {
        Expression* l_expression = nullptr;
        std::uint32_t operator_token = -1;
        Expression* r_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Binary_text_operator_expression : Expression {
        Expression* l_expression = nullptr;
        Text* operator_text = nullptr;
        Expression* r_expression = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Function_call : Expression {
        Expression* name = nullptr;
        std::uint32_t l_paren_token = -1;
        Expression_list* parameters = nullptr;
        std::uint32_t r_paren_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Subscript_call : Expression {
        Resolved_identifier* name = nullptr;
        std::uint32_t l_square_bracket_token = -1;
        Expression_list* parameter = nullptr;
        std::uint32_t r_square_bracket_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Subscript_range : Expression {
        Resolved_identifier* name = nullptr;
        std::uint32_t l_square_bracket_token = -1;
        Expression* expression0 = nullptr;
        std::uint32_t first_colon_token = -1;
        Expression* expression1 = nullptr;
        std::uint32_t second_colon_token = -1;
        Expression* expression2 = nullptr;
        std::uint32_t r_square_bracket_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Return_statement : Statement {
        Text* return_keyword = nullptr;
        Expression* expression = nullptr;
        std::uint32_t semicolon_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Variable_declaration : Statement {
        Text* var_keyword = nullptr;
        Type_expression* type = nullptr;
        std::uint32_t equals_token = -1;
        Expression* initializer = nullptr;
        std::uint32_t semicolon_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Constant_declaration : Statement {
        Text* let_keyword = nullptr;
        Type_expression* type = nullptr;
        std::uint32_t equals_token = -1;
        Expression* initializer = nullptr;
        std::uint32_t semicolon_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Module_declaration : Parse_tree_node {
        Text* module_keyword = nullptr;
        Resolved_identifier* module_name = nullptr;
        std::uint32_t semicolon_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Attribute : Parse_tree_node {
        Text* name = nullptr;
        std::uint32_t l_paren_token = -1;
        Expression* value = nullptr;
        std::uint32_t r_paren_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Attribute_list_body : Parse_tree_node {
        Attribute* head = nullptr;
        std::uint32_t comma_token = -1;
        Attribute_list_body* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Attribute_list : Parse_tree_node {
        std::uint32_t l_square_bracket_token = -1;
        Attribute_list_body* body = nullptr;
        std::uint32_t r_square_bracket_token = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Statement_sequence : Parse_tree_node {
        Statement* head = nullptr;
        Code_body* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Code_body : Statement {
        std::uint32_t l_curly_bracket = -1;
        Statement_sequence* statement_sequence = nullptr;
        std::uint32_t r_curly_bracket = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }

    };

    struct Parameter : Parse_tree_node {
        Text* input_qualifier = nullptr;
        Type_expression* type = nullptr;
        Text* name = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Parameter_sequence : Parse_tree_node {
        Parameter* head = nullptr;
        std::uint32_t comma_token = -1;
        Parameter* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Parameter_list : Parse_tree_node {
        std::uint32_t l_paren = -1;
        Parameter_sequence* parameter_sequence = nullptr;
        std::uint32_t r_paren = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Source_body : Parse_tree_node {

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }

    };

    struct Function_definition : Source_body {
        Text* func_keyword = nullptr;
        Attribute_list* attributes = nullptr;
        Text* name = nullptr;
        std::uint32_t l_paren_token = -1;
        Parameter_list* parameter_list = nullptr;
        std::uint32_t r_paren_token = -1;
        Type_expression* return_type = nullptr;
        Code_body* body = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Lambda : Expression {
        Text* lambda_keyword = nullptr;
        Attribute_list* attribute_list = nullptr;
        Text* name = nullptr;
        std::uint32_t l_paren_token = -1;
        Parameter_list* parameter_list = nullptr;
        std::uint32_t r_paren_token = -1;
        Type_expression* return_type = nullptr;
        std::uint32_t l_curly_bracket = -1;
        Code_body* body = nullptr;
        std::uint32_t r_curly_bracket = -1;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Struct_definition : Source_body {
        Text* struct_keyword = nullptr;
        Attribute_list* attribute_list = nullptr;
        //TODO: Complete implementation

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Alias_definition : Source_body, Statement {
        Text* alias_keyword = nullptr;
        Attribute_list* attribute_list = nullptr;
        std::uint32_t equals_token = -1;
        Type_expression* type = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Source_body_list : Parse_tree_node {
        Source_body* head = nullptr;
        Source_body_list* tail = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Source_file : Parse_tree_node {
        Module_declaration* module_declaration = nullptr;
        Source_body_list* body = nullptr;

        void accept(Visitor& visitor) override {
            visitor.visit(this);
        }
    };

    struct Parse_tree {
        //TODO: Handle deleting nodes in constructor
        Parse_tree_node* root;
    };

}

#endif //HARC_PARSE_TREE_HPP
