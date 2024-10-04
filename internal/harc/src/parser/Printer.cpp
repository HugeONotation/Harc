#include <harc/parser/Printer.hpp>

#include <harc/parser/Parse_tree_visitor.hpp>

namespace harc::parser {

    class Printer : public Visitor {
    public:

        //=================================================
        // -ctors
        //=================================================

        explicit Printer(const Translation_unit &unit) :
            unit(unit),
            indentation_buffer(256, ' ') {}

        //=================================================
        // Accessors
        //=================================================

        std::string&& print() {
            if (unit.parse_tree.root) {
                unit.parse_tree.root->accept(*this);
            }

            return std::move(output);
        }

        //=================================================
        // Visit methods
        //=================================================

        void visit(std::uint32_t token_index) override {
            if (token_index == -1) {
                return;
            }

            auto token_type = unit.tokenization.types[token_index];

            indent();
            output += to_string(token_type);

            if (is_textual(token_type)) {
                output += ' ';
                output += unit.token_source(token_index);
            }

            output += '\n';
        }

        void visit(Parse_tree_node* node) override {
            // Don't do anything
        }

        void visit(Text* node) override {
            indent();
            output += "TEXT: ";
            //TODO: Escape text
            output += unit.token_source(node->text_token);
            output += '\n';
        }

        void visit(Resolved_identifier* node) override {
            indent();
            output += "RESOLVED_IDENTIFIER:\n";
            depth += 1;

            if (node->head) {
                node->head->accept(*this);
            }

            visit(node->dot_token);

            if (node->tail) {
                node->tail->accept(*this);
            }

            depth -= 1;
            if (output.back() != '\n') {
                output += '\n';
            }
        }

        void visit(Identifier* node) override {
            indent();
            output += "IDENTIFIER:\n";
            depth += 1;

            if (node->name) {
                node->name->accept(*this);
            }

            depth -=1;
            output += '\n';
        }

        void visit(Expression* node) override {
            // Nothing to do
        }

        void visit(Expression_list* node) override {
            indent();
            output += "EXPRESSION_LIST:\n";
            depth += 1;

            if (node->head) {
                node->head->accept(*this);
            }

            visit(node->semicolon_token);

            if (node->tail) {
                visit(node->tail);
            }

            depth -= 1;
            output += '\n';
        }

        void visit(Function_call* node) override {
            indent();
            output += "FUNCTION_CALL:\n";
            depth += 1;

            if (node->name) {
                node->name->accept(*this);
            }

            visit(node->l_paren_token);

            if (node->parameters) {
                node->parameters->accept(*this);
            }

            visit(node->r_paren_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Statement* node) override {
            // Nothing to do
        }

        void visit(Return_statement* node) override {
            indent();
            output += "RETURN_STATEMENT:\n";
            depth += 1;

            if (node->return_keyword) {
                node->return_keyword->accept(*this);
            }

            if (node->expression) {
                node->expression->accept(*this);
            }

            visit(node->semicolon_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Variable_declaration* node) override {
            indent();
            output += "VARIABLE_DECLARATION:\n";
            depth += 1;

            if (node->var_keyword) {
                node->var_keyword->accept(*this);
            }

            if (node->type) {
                node->type->accept(*this);
            }

            visit(node->equals_token);

            if (node->initializer) {
                node->initializer->accept(*this);
            }

            visit(node->semicolon_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Constant_declaration* node) override {
            indent();
            output += "CONSTANT_DECLARATION:\n";
            depth += 1;

            if (node->let_keyword) {
                node->let_keyword->accept(*this);
            }

            if (node->type) {
                node->type->accept(*this);
            }

            visit(node->equals_token);

            if (node->initializer) {
                node->initializer->accept(*this);
            }

            visit(node->semicolon_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Module_declaration* node) override {
            indent();
            output += "MODULE_DECLARATION:\n";
            depth += 1;

            if (node->module_keyword) {
                node->module_keyword->accept(*this);
            }

            if (node->module_name) {
                node->module_name->accept(*this);
            }

            visit(node->semicolon_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Attribute* node) override {
            indent();
            output += "ATTRIBUTE:\n";
            depth += 1;

            if (node->name){
                visit(node->name);
            }

            visit(node->l_paren_token);

            if (node->value) {
                visit(node->value);
            }

            visit(node->r_paren_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Attribute_list_body* node) override {
            indent();
            output += "ATTRIBUTE_LIST_BODY:\n";
            depth += 1;

            if (node->head) {
                visit(node->head);
            }

            visit(node->comma_token);

            if (node->tail) {
                visit(node->tail);
            }
            depth -= 1;
            output += '\n';
        }

        void visit(Attribute_list* node) override {
            indent();
            output += "ATTRIBUTE_LIST:\n";
            depth += 1;

            visit(node->l_square_bracket_token);

            if (node->body) {
                node->body->accept(*this);
            }

            visit(node->r_square_bracket_token);

            depth -= 1;
            output += '\n';
        }

        void visit(Statement_sequence* node) override {
            indent();
            output += "ATTRIBUTE_LIST:\n";
            depth += 1;

            if (node->head) {
                node->head->accept(*this);
            }

            if (node->tail) {
                node->tail->accept(*this);
            }

            depth -= 1;
            if (output.back() != '\n') {
                output += '\n';
            }
        }

        void visit(Code_body* node) override {
            indent();
            output += "CODE_BODY:\n";
            depth += 1;

            visit(node->l_curly_bracket);

            if (node->statement_sequence) {
                node->statement_sequence->accept(*this);
            }

            visit(node->r_curly_bracket);

            depth -= 1;
            output += '\n';
        }

        void visit(Parameter* node) override {
            indent();
            output += "PARAMETER:\n";
            depth += 1;

            if (node->input_qualifier) {
                node->input_qualifier->accept(*this);
            }

            if (node->type) {
                node->type->accept(*this);
            }

            if (node->name) {
                node->name->accept(*this);
            }

            depth -= 1;
            output += '\n';
        }

        void visit(Parameter_sequence* node) override {
            indent();
            output += "PARAMETER_SEQUENCE:\n";
            depth += 1;

            if (node->head) {
                node->head->accept(*this);
            }

            visit(node->comma_token);

            if (node->tail) {
                node->tail->accept(*this);
            }

            depth -= 1;
            if (output.back() != '\n') {
                output += '\n';
            }
        }

        void visit(Parameter_list* node) override {
            indent();
            output += "PARAMETER_LIST:\n";
            depth += 1;

            visit(node->l_paren);

            if (node->parameter_sequence) {
                node->parameter_sequence->accept(*this);
            }

            visit(node->r_paren);

            depth -= 1;
            output += '\n';
        }

        void visit(Source_body* node) override {

        }

        void visit(Function_definition* node) override {
            indent();
            output += "FUNCTION_DEFINITION:\n";
            depth += 1;

            if (node->func_keyword) {
                node->func_keyword->accept(*this);
            }

            if (node->attributes) {
                node->attributes->accept(*this);
            }

            if (node->name) {
                node->name->accept(*this);
            }

            if (node->parameter_list) {
                node->parameter_list->accept(*this);
            }

            if (node->return_type) {
                node->return_type->accept(*this);
            }

            if (node->body) {
                node->body->accept(*this);
            }

            depth -= 1;
            output += '\n';
        }

        void visit(Lambda* node) override {
            indent();
            output += "LAMBDA:\n";
            depth += 1;

            if (node->lambda_keyword) {
                node->lambda_keyword->accept(*this);
            }

            if (node->attribute_list) {
                node->attribute_list->accept(*this);
            }

            if (node->name) {
                node->name->accept(*this);
            }

            visit(node->l_paren_token);

            if (node->parameter_list) {
                node->parameter_list->accept(*this);
            }

            visit(node->r_paren_token);


            if (node->return_type) {
                node->return_type->accept(*this);
            }

            visit(node->l_curly_bracket);

            if (node->body) {
                node->body->accept(*this);
            }

            visit(node->r_curly_bracket);

            depth -= 1;
            output += '\n';
        }

        void visit(Struct_definition* node) override {
            indent();
            output += "CLASS_DEFINITION:\n";
            depth += 1;

            if (node->struct_keyword) {
                node->struct_keyword->accept(*this);
            }

            if (node->attribute_list) {
                node->attribute_list->accept(*this);
            }

            depth -= 1;
            output += '\n';
        }

        void visit(Alias_definition* node) override {
            indent();
            output += "ALIAS_DEFINITION:\n";
            depth += 1;

            if (node->alias_keyword) {
                node->alias_keyword->accept(*this);
            }

            if (node->attribute_list) {
                node->attribute_list->accept(*this);
            }

            visit(node->equals_token);

            if (node->type) {
                node->type->accept(*this);
            }

            depth -= 1;
            output += '\n';
        }

        void visit(Source_body_list* node) override {
            indent();
            output += "SOURCE_BODY_LIST:\n";
            depth += 1;

            if (node->head) {
                node->head->accept(*this);
            }

            if (node->tail) {
                node->tail->accept(*this);
            }

            depth -= 1;
            if (output.back() != '\n') {
                output += '\n';
            }
        }

        void visit(Source_file* node) override {
            indent();
            output += "SOURCE_FILE:\n";
            depth += 1;
            if (node->module_declaration) {
                node->module_declaration->accept(*this);
            }

            if (node->body) {
                node->body->accept(*this);
            }

            depth -= 1;
            output += '\n';
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint32_t depth = 0;

        const Translation_unit &unit;

        std::string indentation_buffer;

        std::string output;

        //=================================================
        // Instance members
        //=================================================

        void indent() {
            auto space_count = depth * 4;
            if (space_count > indentation_buffer.size()) {
                indentation_buffer.resize(space_count, ' ');
            }

            std::string_view indentation{
                indentation_buffer.data() + indentation_buffer.size() - space_count,
                space_count
            };

            output += indentation;
        }

    };

}

namespace harc {

    std::string parse_tree_to_string(const Translation_unit &unit) {
        parser::Printer printer{unit};
        auto ret = printer.print();
        return ret;
    }

}
