#include <harc/parser/Parser.hpp>
#include <harc/parser/Operators.hpp>
#include <harc/lexer/Tokens.hpp>
#include <harc/Error_reporting.hpp>

#include <vector>

namespace harc::parser {

    /*
    class Parser {
    public:

        //=================================================
        // -ctors
        //=================================================

        explicit Parser(Translation_unit& unit):
            unit(unit),
            token_index(0),
            end_index(unit.tokenization.types.size()) {}

        //=================================================
        // Mutators
        //=================================================

        Error_code parse() {
            unit.parse_tree.root = parse_source_file();
            return error_code;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Translation_unit& unit;

        std::uint32_t token_index = 0;

        const std::uint32_t end_index;

        Error_code error_code = Error_code::NO_ERROR;

        //=================================================
        // Parsing functions
        //=================================================

        Text* parse_text(std::string_view expected, bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            if (!expected.empty()) {
                if (unit.tokenization.types[token_index] != Token_type::TEXT) {
                    std::string message = "Expected \"";
                    message.append(expected);
                    message.push_back('\"');
                    report_parsing_error(message);
                    return nullptr;
                }

                std::string_view view{
                    unit.source.data() + unit.tokenization.source_indices[token_index],
                    unit.tokenization.lengths[token_index]
                };

                if (view != expected) {
                    if (is_required) {
                        std::string message = "Expected \"";
                        message.append(expected);
                        message.push_back('\"');
                        report_parsing_error(message);
                    }

                    return nullptr;
                }
            }

            auto ret = new Text();

            ret->text_token = token_index;
            ++token_index;
            return ret;
        }

        Text* parse_text(const char* expected, bool is_required) {
            return parse_text(std::string_view{expected}, is_required);
        }

        Text* parse_text(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            if (is_required) {
                if (unit.tokenization.types[token_index] != Token_type::TEXT) {
                    std::string message = "Expected textual token.";
                    report_parsing_error(message);
                    return nullptr;
                }
            }

            auto ret = new Text();

            ret->text_token = token_index;
            ++token_index;
            return ret;
        }

        Resolved_identifier* parse_resolved_identifier(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Resolved_identifier();
            ret->head = parse_text(is_required);

            if (!ret->head) {
                report_parsing_error("Expected textual token.");

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::PERIO) {
                return ret;
            }
            ret->dot_token = token_index;
            ++token_index;

            ret->tail = parse_resolved_identifier(true);
            if (!ret->tail) {
                report_parsing_error("Expected rest of resolved identifier.");

                delete ret;
                return nullptr;
            }

            return ret;
        }

        Expression* parse_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Expression();

            std::vector<Expression*> operand_stack;
            std::vector<Token_type> unary_operator_stack;
            std::vector<Token_type> binary_operator_stack;

            //Utilize variant of Shunting Yard algorithm
            while (true) {
                auto curr = current_token_type();

                if (is_unary_operator(current_token_type())) {
                    unary_operator_stack.push_back(current_token_type());
                    continue;
                }

                if (unit.tokenization.types[token_index] == Token_type::L_PAREN) {

                    continue;
                }

                if (unit.tokenization.types[token_index] == Token_type::TEXT && unit.token_source(token_index) == "lambda") {
                    operand_stack.push_back(parse_lambda(true));
                    continue;
                }

                if (is_binary_operator(current_token_type())) {

                }

                increment_token();

                auto next = current_token_type();

                // Check for function call
                if (curr == Token_type::TEXT && next == Token_type::L_PAREN) {

                }

                break;
            }

            //TODO: Reconstruct parse tree

            return ret;
        }

        Expression_list* parse_expression_list(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Expression_list();
            ret->head = parse_expression(is_required);
            if (!ret->head) {
                if (is_required) {
                    report_parsing_error("Expected expression");
                }

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::SEMICOLON) {
                return ret;
            }
            ret->semicolon_token = token_index;
            token_index += 1;

            ret->tail = parse_expression_list(false);

            return ret;
        }

        Expression_sequence* parse_expression_sequence(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }


            //TODO: Implement
            return nullptr;
        }

        Type_expression* parse_type_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            //TODO: Implement
            return nullptr;
        }

        Resolved_template* parse_resolved_template(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            //TODO: Implement
            return nullptr;
        }



        Numeric_literal_expression* parse_numeric_literal_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Numeric_literal_expression();

            if (unit.tokenization.types[token_index] != Token_type::NUMERIC_LITERAL) {
                if (is_required) {
                    report_parsing_error("Expected numeric literal.");
                }

                delete ret;
                return nullptr;
            }
            ret->numeric_literal_token = token_index;
            token_index += 1;

            if (unit.tokenization.types[token_index] != Token_type::COLON) {
                return ret;
            }
            ret->colon_token = token_index;
            token_index += 1;

            ret->type_expression = parse_expression(true);
            if (!ret->type_expression) {
                report_parsing_error("Expected type suffix after numeric literal.");
                delete ret;
                return nullptr;
            }

            return ret;
        }

        String_literal_expression* parse_string_literal_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new String_literal_expression();

            if (unit.tokenization.types[token_index] != Token_type::STRING_LITERAL) {
                if (is_required) {
                    report_parsing_error("Expected string literal");
                }

                delete ret;
                return nullptr;
            }
            ret->string_literal_token = token_index;
            token_index += 1;

            if (unit.tokenization.types[token_index] != Token_type::COLON) {
                return ret;
            }
            ret->colon_token = token_index;
            token_index += 1;

            ret->type_expression = parse_expression(true);
            if (!ret->type_expression) {
                report_parsing_error("Expected type suffix after string literal.");
                delete ret;
                return nullptr;
            }

            return ret;
        }

        Codepoint_literal_expression* parse_codepoint_literal_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Codepoint_literal_expression();

            if (unit.tokenization.types[token_index] != Token_type::CHAR_LITERAL) {
                if (is_required) {
                    report_parsing_error("Expected codepoint literal");
                }

                delete ret;
                return nullptr;
            }
            ret->codepoint_literal_token = token_index;
            token_index += 1;

            if (unit.tokenization.types[token_index] != Token_type::COLON) {
                return ret;
            }
            ret->colon_token = token_index;
            token_index += 1;

            ret->type_expression = parse_expression(true);
            if (!ret->type_expression) {
                report_parsing_error("Expected type suffix after codepoint literal.");
                delete ret;
                return nullptr;
            }

            ret->type_expression = parse_expression(true);

            return ret;
        }

        Tuple_literal* parse_tuple_literal(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Parenthesized_expression* parse_parenthesized_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Unary_operator_expression* parse_unary_operator_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Binary_operator_expression* parse_binary_operator_expression(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Binary_text_operator_expression* parse_binary_text_operator(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Attribute* parse_attribute(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Attribute();
            ret->name = parse_text(is_required);
            if (!ret->name) {
                if (is_required) {
                    report_parsing_error("Expected attribute name");
                }

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::L_PAREN) {
                return ret;
            }
            ret->l_paren_token = token_index;
            token_index += 1;

            ret->value = parse_expression(true);

            if (unit.tokenization.types[token_index] != Token_type::R_PAREN) {
                report_parsing_error("Expected ) to close attribute specification");

                delete ret;
                return nullptr;
            }
            ret->r_paren_token = token_index;
            token_index += 1;

            return nullptr;
        }

        Attribute_list_body* parse_attribute_list_body(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Attribute_list_body();
            ret->head = parse_attribute(false);
            if (!ret->head) {
                if (is_required) {
                    report_parsing_error("Expected attribute");
                }

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::COMMA) {
                return ret;
            }
            ret->comma_token = token_index;
            token_index += 1;

            ret->tail = parse_attribute_list_body(false);

            return ret;
        }

        Attribute_list* parse_attribute_list(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Attribute_list();

            if (unit.tokenization.types[token_index] != Token_type::L_SQUARE_BRACKET) {
                if (is_required) {
                    report_parsing_error("Expected [ as opening for attribute list");
                }

                delete ret;
                return nullptr;
            }
            token_index += 1;

            ret->body = parse_attribute_list_body(false);

            if (unit.tokenization.types[token_index] != Token_type::L_SQUARE_BRACKET) {
                if (is_required) {
                    report_parsing_error("Expected ] as closing for attribute list");
                }

                delete ret;
                return nullptr;
            }
            token_index += 1;

            return ret;
        }

        Parameter_list* parse_parameter_list(bool is_required) {
            //TODO: Implement
            return nullptr;
        }

        Function_definition* parse_function_definition(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Function_definition();
            if (unit.tokenization.types[token_index] != Token_type::TEXT) {
                if (is_required) {
                    report_parsing_error("Expected func keyword");
                }

                delete ret;
                return nullptr;
            }
            token_index += 1;

            ret->attributes = parse_attribute_list(false);

            ret->name = parse_text(true);
            if (!ret->name) {
                report_parsing_error("Expected function name");

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::L_PAREN) {
                report_parsing_error("Expected opening ( for function parameter list");

                delete ret;
                return nullptr;
            }
            token_index += 1;

            ret->parameter_list = parse_parameter_list(false);

            if (unit.tokenization.types[token_index] != Token_type::R_PAREN) {
                report_parsing_error("Expected closing ) for function parameter list");

                delete ret;
                return nullptr;
            }
            token_index += 1;

            ret->return_type = parse_type_expression(true);
            if (!ret->return_type) {
                report_parsing_error("Expected function return type");

                delete ret;
                return nullptr;
            }




            //TODO: Complete implementation

            return ret;
        }

        Lambda* parse_lambda(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Lambda();
            ret->lambda_keyword = parse_text("lambda", is_required);


            return ret;
        }

        Struct_definition* parse_struct_definition(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            return nullptr;
        }

        Module_declaration* parse_module_declaration(bool is_required) {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Module_declaration();
            ret->module_keyword = parse_text("module");
            if (!ret->module_keyword) {
                delete ret;

                if (!is_required) {
                    return nullptr;
                }

                report_parsing_error("Expected module keyword");

                return nullptr;
            }

            ret->module_name = parse_resolved_identifier(true);
            if (!ret->module_name) {
                report_parsing_error("Expected module name");

                delete ret;
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::SEMICOLON) {
                report_parsing_error("Expected semicolon.");

                delete ret;
                return nullptr;
            }
            ret->semicolon_token = token_index;
            ++token_index;

            return ret;
        }

        Source_body* parse_source_body() {
            if (token_index == end_index) {
                return nullptr;
            }

            if (unit.tokenization.types[token_index] != Token_type::TEXT) {
                return nullptr;
            }

            std::string_view view{
                unit.source.data() + unit.tokenization.source_indices[token_index],
                unit.tokenization.lengths[token_index]
            };

            if (view == "func") {
                return parse_function_definition(true);
            } else {
                return nullptr;
            }
        }

        Source_body_list* parse_source_body_list() {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Source_body_list();
            ret->head = parse_source_body();
            if (!ret->head) {
                delete ret;
                return nullptr;
            }

            ret->tail = parse_source_body_list();

            return ret;
        }

        Parse_tree_node* parse_source_file() {
            if (token_index == end_index) {
                return nullptr;
            }

            auto* ret = new Source_file();

            // Module declaration is required
            ret->module_declaration = parse_module_declaration(true);
            if (!ret->module_declaration) {
                delete ret;

                report_parsing_error("Expected module declaration");
                return nullptr;
            }

            // Body can be empty
            ret->body = parse_source_body_list();

            return ret;
        }

        //=================================================
        // Helper functions
        //=================================================

        Token_type current_token_type() {
            return unit.tokenization.types[token_index];
        }

        void increment_token() {
            ++token_index;
        }

    };
    */

    Error_code parse(Translation_unit& unit) {
        //Parser parser{unit};
        //auto error_code = parser.parse();
        return {};
    }

}
