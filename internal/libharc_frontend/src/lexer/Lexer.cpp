#include <harc/lexer/Lexer.hpp>
#include <harc/lexer/Tokens.hpp>

//#include <harc/Logging.hpp>
//#include <harc/Error_reporting.hpp>

#include <vector>
#include <bitset>
#include <cstdlib>
#include <utility>

namespace harc::lex {

    constexpr std::uint8_t joined = 0x00; // No whitespace on either side
    constexpr std::uint8_t prefix = 0x01; // No whitespace after
    constexpr std::uint8_t suffix = 0x02; // No whitespace before
    constexpr std::uint8_t spaced = 0x03; // Whitespace on both sides
    constexpr std::uint8_t unused = 0x04; // Unused
    constexpr std::uint8_t accord = 0x05; // Whitespace on both sides or neither
    constexpr std::uint8_t attach = 0x06; // Whitespace on at least one side
    constexpr std::uint8_t ignore = 0x07; // Token is not whitespace sensitive

    constexpr std::array<std::uint8_t, 3> encode(char x, char y, char z, std::uint8_t sensitivity) {
        return std::array<std::uint8_t, 3> {
            std::uint8_t(x | ((sensitivity & 0x01) << 7)),
            std::uint8_t(y | ((sensitivity & 0x02) << 6)),
            std::uint8_t(z | ((sensitivity & 0x04) << 5))
        };

    }

    // Table containing the representation for all non-textual codepoints
    // Each sequence of three bytes encode the token's representation
    // The low 7 bits of each byte indicate the codepoint value that is
    // required to comprise the operator.
    //
    // This list is ordered by the codepoint value of the nth token, with
    // longer tokens being placed first so that in a linear scan, the
    // longest match occurs first.
    constexpr std::array<std::array<std::uint8_t, 3>, 69> token_table {
        encode(0x0,  0x0, 0x0,      0), // 0x00 NULL_TOKEN
        encode('!',  '=', 0x0, spaced), // 0x01 SPACED_COMPARE_NE
        encode('!',  0x0, 0x0, prefix), // 0x02 PREFIX_EXCLAMATION
        encode('#',  0x0, 0x0, ignore), // 0x03 HASH
        encode('%',  '=', 0x0, spaced), // 0x04 SPACED_REM_EQUALS
        encode('%',  0x0, 0x0, spaced), // 0x05 SPACED_PERCENT
        encode('&',  '&', '=', spaced), // 0x06 SPACED_LOGICAL_AND_EQUALS
        encode('&',  '&', 0x0, spaced), // 0x07 SPACED_LOGICAL_AND
        encode('&',  '=', 0x0, spaced), // 0x08 SPACED_BITWISE_AND_EQUALS
        encode('&',  0x0, 0x0, spaced), // 0x09 SPACED_AMPERSAND
        encode('&',  0x0, 0x0, suffix), // 0x0a SUFFIX_AMPERSAND
        encode('(',  0x0, 0x0, ignore), // 0x0b LEFT_PARENTHESIS
        encode(')',  0x0, 0x0, ignore), // 0x0c RIGHT_PARENTHESIS
        encode('*',  '/', 0x0, ignore), // 0x0d RIGHT_COMMENT_BRACKET
        encode('*',  '=', 0x0, spaced), // 0x0e SPACED_TIMES_EQUALS
        encode('*',  0x0, 0x0, spaced), // 0x0f SPACED_ASTERISK
        encode('+',  '+', 0x0, prefix), // 0x10 PREFIX_INCREMENT
        encode('+',  '+', 0x0, suffix), // 0x11 SUFFIX_INCREMENT
        encode('+',  '=', 0x0, spaced), // 0x12 SPACED_PLUS_EQUALS
        encode('+',  0x0, 0x0, spaced), // 0x13 SPACED_PLUS
        encode('+',  0x0, 0x0, spaced), // 0x14 PREFIX_PLUS
        encode(',',  0x0, 0x0, ignore), // 0x15 COMMA
        encode('-',  '-', 0x0, prefix), // 0x16 PREFIX_DECREMENT
        encode('-',  '-', 0x0, suffix), // 0x17 SUFFIX_DECREMENT
        encode('-',  '=', 0x0, spaced), // 0x18 SPACED_MINUS_EQUALS
        encode('-',  '>', 0x0, joined), // 0x19 JOINED_SINGLE_ARROW
        encode('-',  0x0, 0x0, spaced), // 0x1a SPACED_MINUS
        encode('-',  0x0, 0x0, prefix), // 0x1b PREFIX_MINUS
        encode('.',  '.', '.', suffix), // 0x1c SUFFIX_TRIPLE_PERIOD
        encode('.',  '.', 0x0, joined), // 0x1d JOINED_DOUBLE_PERIOD
        encode('.',  0x0, 0x0, joined), // 0x1e JOINED_PERIOD
        encode('/',  '*', 0x0, ignore), // 0x1f LEFT_COMMENT_BRACKET
        encode('/',  '=', 0x0, spaced), // 0x20 SPACED_DIVIDE_EQUALS
        encode('/',  0x0, 0x0, spaced), // 0x21 SPACED_SLASH
        encode(':',  ':', 0x0, joined), // 0x22 JOINED_DOUBLE_COLON
        encode(':',  0x0, 0x0, accord), // 0x23 COLON
        encode(';',  0x0, 0x0, ignore), // 0x24 SEMICOLON
        encode('<',  '|', '=', spaced), // 0x25 SPACED_SATURATING_LEFT_SHIFT_EQUALS
        encode('<',  '<', '=', spaced), // 0x26 SPACED_MODULAR_LEFT_SHIFT_EQUALS
        encode('<',  '<', 0x0, spaced), // 0x27 SPACED_MODULAR_LEFT_SHIFT
        encode('<',  '=', 0x0, spaced), // 0x28 SPACED_COMPARE_LE
        encode('<',  '|', 0x0, spaced), // 0x29 SPACED_SATURATING_LEFT_SHIFT
        encode('<',  0x0, 0x0, spaced), // 0x2a SPACED_COMPARE_LT
        encode('<',  0x0, 0x0, attach), // 0x2b LEFT_TEMPLATE_BRACKET
        encode('=',  '=', 0x0, spaced), // 0x2c SPACED_COMPARE_EQ
        encode('=',  '>', 0x0, spaced), // 0x2d SPACED_DOUBLE_ARROW
        encode('=',  0x0, 0x0, spaced), // 0x2e SPACED_EQUALS
        encode('>',  '>', '=', spaced), // 0x2f SPACED_MODULAR_RIGHT_SHIFT_EQUALS
        encode('>',  '=', 0x0, spaced), // 0x30 SPACED_COMPARE_GE
        encode('>',  '>', 0x0, spaced), // 0x31 SPACED_MODULAR_RIGHT_SHIFT
        encode('>',  0x0, 0x0, spaced), // 0x32 SPACED_COMPARE_GT
        encode('>',  0x0, 0x0, attach), // 0x33 RIGHT_TEMPLATE_BRACKET
        encode('@',  0x0, 0x0, ignore), // 0x34 AT_SIGN
        encode('[',  0x0, 0x0, ignore), // 0x35 LEFT_SQUARE_BRACKET
        encode(']',  0x0, 0x0, ignore), // 0x36 RIGHT_SQUARE_BRACKET
        encode('^',  '^', '=', spaced), // 0x37 SPACED_LOGICAL_XOR_EQUALS
        encode('^',  '=', 0x0, spaced), // 0x38 SPACED_BITWISE_XOR_EQUALS
        encode('^',  '^', 0x0, spaced), // 0x39 SPACED_LOGICAL_XOR
        encode('^',  0x0, 0x0, spaced), // 0x3a SPACED_CARET
        encode('^',  0x0, 0x0, suffix), // 0x3b SUFFIX_CARET
        encode('{',  0x0, 0x0, ignore), // 0x3c LEFT_CURLY_BRACKET
        encode('|',  '>', '=', spaced), // 0x3d SPACED_SATURATING_RIGHT_SHIFT_EQUALS
        encode('|',  '|', '=', spaced), // 0x3e SPACED_LOGICAL_OR_EQUALS
        encode('|',  '=', 0x0, spaced), // 0x3f SPACED_BITWISE_OR_EQUALS
        encode('|',  '|', 0x0, spaced), // 0x40 SPACED_LOGICAL_OR
        encode('|',  '>', 0x0, spaced), // 0x41 SPACED_SATURATING_RIGHT_SHIFT
        encode('|',  0x0, 0x0, spaced), // 0x42 SPACED_PIPE
        encode('}',  0x0, 0x0, ignore), // 0x43 RIGHT_CURLY_BRACKET
        encode('~',  0x0, 0x0, prefix)  // 0x44 PREFIX_TILDE
    };





    ///
    ///
    ///
    class Tokenizer {
    private:

        //=================================================
        // Helper classes
        //=================================================

        struct Stack_entry {
            Token_type type;
            std::uint32_t index;
        };

        //=================================================
        // Instance members
        //=================================================

        std::string_view source_id{};
        std::string_view source{};
        const Config* config{};

        Tokenization ret;

        std::array<char, 5> window{};
        std::uint32_t byte_index = 0;
        std::bitset<5> is_ws = 0;
        const char* it = nullptr;
        std::array<std::uint32_t, 2> pos{};
        std::uint32_t line_begin_index = 0;
        std::vector<Stack_entry> balancing_stack{};

    public:

        //=================================================
        // -ctors
        //=================================================

        Tokenizer(
            std::string_view source_id,
            std::string_view source,
            const Config& config
        ):
            source_id(source_id),
            source(source),
            config(&config)
        {}

        //=================================================
        // Misc.
        //=================================================

        std::uint32_t estimate_bytes_per_token() {
            const std::uint32_t bytes_per_token_estimate = 5;
            auto token_count_estimate = source.size() / bytes_per_token_estimate;

            return token_count_estimate;
        }

        std::uint32_t estimate_line_count() {
            const std::uint32_t bytes_per_line_estimate = 8;
            auto line_count_estimate = source.size() / bytes_per_line_estimate;

            return line_count_estimate;
        }

        void emplace_token(
            Token_type type,
            std::uint32_t index,
            std::uint32_t length,
            std::array<std::uint32_t, 2> location,
            std::uint32_t pair_index
        ) {
            ret.types.push_back(type);
            ret.source_indices.push_back(index);
            ret.lengths.push_back(length);
            ret.pair_indices.push_back(index);
        }

        void emplace_token(
            Token_type type,
            std::uint32_t index,
            std::uint32_t length,
            std::array<std::uint32_t, 2> location
        ) {
            emplace_token(type, index, length, location, ret.pair_indices.size());
        }

        static bool is_whitespace(char c) {
            return
                c == ' '  ||
                c == '\n' ||
                c == '\t' ||
                c == '\v';
        }

        void advance_window(unsigned o = 1) {
            if (source.end() - it < o) [[unlikely]] {
                return; //Special handling
            }

            // Skip forward the indicated amount
            std::shift_left(window.begin(), window.end(), o);
            is_ws >>= o;

            for (std::uint32_t i = 5 - o; i < 5; ++i) {
                window[i] = *it;
                ++it;
                ++byte_index;
                is_ws[i] = is_whitespace(window[i]);
            }

            // Skip whitespace
            while (is_ws[1] && (it != source.end())) {
                std::shift_left(window.begin(), window.end(), 1);
                is_ws >>= 1;

                window[4] = *it;
                ++it;
                is_ws[4] = is_whitespace(window[4]);

                // Special handling for newlines
                if (window[1] == '\n') {
                    pos[0] +=  1;
                    pos[1] = 0;
                    line_begin_index = it - source.begin() - 4;
                    ret.line_indices.push_back(line_begin_index);
                }
            }
        }

        static constexpr std::array<std::uint8_t, 3> encode_token(char x, char y, char z, std::uint8_t sensitivity) {
            return std::array<std::uint8_t, 3> {
                std::uint8_t(x | ((sensitivity & 0x01) << 7)),
                std::uint8_t(y | ((sensitivity & 0x02) << 6)),
                std::uint8_t(z | ((sensitivity & 0x04) << 5))
            };

        }

        static bool test_whitespace(bool a, bool b, bool c, bool x, bool y) {
            //TODO: Simplify this further
            bool t0 = a & b;
            bool t1 = x | y;

            bool s0 = !((x ^ a) | (y | c) | b);
            bool s1 = t0 & (c | t1);
            bool s2 = (t0 & !c) & t1;

            return s0 | s1 | s2;
        }

        Token_type identify_next_token() {
            // TODO: Address the large amount of branching
            if (('0' <= window[1]) && (window[1] <= '9')) {
                return Token_type::NUMERIC_LITERAL;
            }

            if (
                (('a' <= window[1]) && (window[1] <= 'z')) ||
                (('A' <= window[1]) && (window[1] <= 'Z')) ||
                window[1] == '_'
            ) {
                return Token_type::TEXT;
            }

            if (window[1] == '/' && window[2] == '/') {
                if (window[3] == '/') {
                    return Token_type::DOC_TEXT;
                } else {
                    return Token_type::SINGLE_LINE_COMMENT;
                }
            }

            if (window[1] == '\"') {
                return Token_type::STRING_LITERAL;
            }

            if (window[1] == '\'') {
                return Token_type::CODEPOINT_LITERAL;
            }

            Token_type type =  Token_type::NULL_TOKEN;
            for (std::size_t i = 1; i < token_table.size(); ++i) {
                std::uint8_t cdpt0 = token_table[i][0] & 0x7f;
                std::uint8_t cdpt1 = token_table[i][1] & 0x7f;
                std::uint8_t cdpt2 = token_table[i][2] & 0x7f;

                bool codepoint_match =
                    (cdpt0 == 0x0) | (cdpt0 == window[1]) &
                        (cdpt1 == 0x0) | (cdpt1 == window[1]) &
                        (cdpt2 == 0x0) | (cdpt2 == window[1]);

                int end_index = 0;

                bool x = is_ws[0];
                bool y = is_ws[end_index];

                bool a = token_table[i][0] >> 7;
                bool b = token_table[i][1] >> 7;
                bool c = token_table[i][2] >> 7;

                bool whitespace_match = test_whitespace(a, b, c, x, y);

                if (codepoint_match && whitespace_match) {
                    type = static_cast<Token_type>(i);
                    break;
                }
            }

            //TODO: Consider alternatives:
            // Binary search
            // SIMD vectorized search
            return type;
        }

        void consume_single_line_comment() {
            while (true) {
                if (window[1] == 0x00) {
                    return; //Return once end of file is reached
                }

                if (window[1] == '\n') {
                    return;
                }

                advance_window();
            }
        }

        void consume_left_comment_bracket() {
            //TODO: Implement
        }

        void consume_doc_text() {
            //TODO: Implement
        }

        void consume_text_token() {
            std::uint32_t starting_index = byte_index;
            while (true) {
                if (window[1] == 0x00) {
                    break; // Return once end of file is reached
                }

                bool is_digit = '0' <= window[1] && window[1] <= '9';
                bool is_lowercase = 'a' <= window[1] && window[1] <= 'z';
                bool is_uppercase = 'A' <= window[1] && window[1] <= 'Z';
                bool is_underscore = window[1] == '_';

                bool is_text_codepoint = is_digit | is_lowercase | is_underscore | is_underscore;
                if (!is_text_codepoint) {
                    break;
                }

                advance_window();
            }

            emplace_token(
                Token_type::TEXT,
                byte_index,
                byte_index - starting_index,
                pos
            );
        }

        void consume_left_token(Token_type type) {
            balancing_stack.emplace_back(type);
            //TODO: Implement
        }

        void consume_right_token(Token_type type) {
            
            //TODO: Implement
        }

        void consume_fixed_length_token(Token_type type) {
            auto width = token_width(type);

            // Handle typical fixed-width tokens
            emplace_token(
                type,
                byte_index,
                width,
                pos
            );

            advance_window(width);
        }

        Tokenization tokenize() {
            ret.source = source;

            // Reserve space for data upfront
            std::uint32_t token_count_estimate = estimate_bytes_per_token();
            ret.types.reserve(token_count_estimate);
            ret.lengths.reserve(token_count_estimate);
            ret.source_indices.reserve(token_count_estimate);

            // Guess number of lines and reserve enough space
            std::uint32_t line_count_estimate = estimate_line_count();
            ret.line_indices.reserve(line_count_estimate);
            ret.line_indices.push_back(0);

            // Guess maximum nesting depth
            balancing_stack.reserve(32);

            // Initialize window and context
            it = source.begin();
            for (int i = 1; i < 5 && (it != source.end()); ++i) {
                window[i] = *it;
                ++it;
                is_ws[i] = is_whitespace(*it);
            }
            byte_index = 0;

            // Core tokenization loop
            while (window[1] != 0x00) {
                Token_type type = identify_next_token();

                if (Token_type::NULL_TOKEN == type) {
                    //TODO: Handle unrecognized token, including non-ascii text
                } else if (Token_type::SINGLE_LINE_COMMENT == type) {
                    consume_single_line_comment();
                } else if (Token_type::LEFT_COMMENT_BRACKET == type) {
                    consume_left_comment_bracket();
                } else if (Token_type::DOC_TEXT == type) {
                    consume_doc_text();
                } else if (is_textual(type)) {
                    consume_text_token();
                } else if (is_left_token(type)) {
                    consume_left_token(type);
                } else if (is_right_token(type)) {
                    consume_right_token(type);
                } else {
                    consume_fixed_length_token(type);
                }
            }

            return std::move(ret);
        }

    };

    /*
    //=========================================================================
    // Tokenization helper functions
    //=========================================================================

    constexpr std::array<std::uint8_t, 3> encode(char x, char y, char z, std::uint8_t sensitivity) {
        return std::array<std::uint8_t, 3> {
            std::uint8_t(x | ((sensitivity & 0x01) << 7)),
            std::uint8_t(y | ((sensitivity & 0x02) << 6)),
            std::uint8_t(z | ((sensitivity & 0x04) << 5))
        };

    }

    bool test_whitespace(bool a, bool b, bool c, bool x, bool y) {
        //TODO: Simplify this further
        bool t0 = a & b;
        bool t1 = x | y;

        bool s0 = !((x ^ a) | (y | c) | b);
        bool s1 = t0 & (c | t1);
        bool s2 = (t0 & !c) & t1;

        return s0 | s1 | s2;
    }

    ///
    /// \param is_last_whitespace Did previous codepoint contain whitespace?
    /// \param w First code point
    /// \param x Second code point
    /// \param y Third code point
    /// \param z Fourth code point
    /// \return Type of token. If the token cannot be categorized, then
    /// Token_type::NULL_TOKEN is returned.
    [[nodiscard]]
    Token_type identify_token(
        const std::array<UChar32, 5> codepoints,
        const std::bitset<5> codepoint_categories
    ) {
        constexpr std::uint8_t joined = 0x00; // No whitespace on either side
        constexpr std::uint8_t prefix = 0x01; // No whitespace after
        constexpr std::uint8_t suffix = 0x02; // No whitespace before
        constexpr std::uint8_t spaced = 0x03; // Whitespace on both sides
        constexpr std::uint8_t unused = 0x04; // Unused
        constexpr std::uint8_t accord = 0x05; // Whitespace on both sides or neither
        constexpr std::uint8_t attach = 0x06; // Whitespace on at least one side
        constexpr std::uint8_t ignore = 0x07; // Token is not whitespace sensitive

        // Table containing the representation for all non-textual codepoints
        // Each sequence of three bytes encode the token's representation
        // The low 7 bits of each byte indicate the codepoint value that is
        // required to comprise the operator.
        //
        // This list is ordered by the codepoint value of the nth token, with
        // longer tokens being placed first so that in a linear scan, the
        // longest match occurs first.
        constexpr std::array<std::array<std::uint8_t, 3>, 69> token_table {
            encode(0x0,  0x0, 0x0,      0), // 0x00 NULL_TOKEN
            encode('!',  '=', 0x0, spaced), // 0x01 SPACED_COMPARE_NE
            encode('!',  0x0, 0x0, prefix), // 0x02 PREFIX_EXCLAMATION
            encode('#',  0x0, 0x0, ignore), // 0x03 HASH
            encode('%',  '=', 0x0, spaced), // 0x04 SPACED_REM_EQUALS
            encode('%',  0x0, 0x0, spaced), // 0x05 SPACED_PERCENT
            encode('&',  '&', '=', spaced), // 0x06 SPACED_LOGICAL_AND_EQUALS
            encode('&',  '&', 0x0, spaced), // 0x07 SPACED_LOGICAL_AND
            encode('&',  '=', 0x0, spaced), // 0x08 SPACED_BITWISE_AND_EQUALS
            encode('&',  0x0, 0x0, spaced), // 0x09 SPACED_AMPERSAND
            encode('&',  0x0, 0x0, suffix), // 0x0a SUFFIX_AMPERSAND
            encode('(',  0x0, 0x0, ignore), // 0x0b LEFT_PARENTHESIS
            encode(')',  0x0, 0x0, ignore), // 0x0c RIGHT_PARENTHESIS
            encode('*',  '/', 0x0, ignore), // 0x0d RIGHT_COMMENT_BRACKET
            encode('*',  '=', 0x0, spaced), // 0x0e SPACED_TIMES_EQUALS
            encode('*',  0x0, 0x0, spaced), // 0x0f SPACED_ASTERISK
            encode('+',  '+', 0x0, prefix), // 0x10 PREFIX_INCREMENT
            encode('+',  '+', 0x0, suffix), // 0x11 SUFFIX_INCREMENT
            encode('+',  '=', 0x0, spaced), // 0x12 SPACED_PLUS_EQUALS
            encode('+',  0x0, 0x0, spaced), // 0x13 SPACED_PLUS
            encode('+',  0x0, 0x0, spaced), // 0x14 PREFIX_PLUS
            encode(',',  0x0, 0x0, ignore), // 0x15 COMMA
            encode('-',  '-', 0x0, prefix), // 0x16 PREFIX_DECREMENT
            encode('-',  '-', 0x0, suffix), // 0x17 SUFFIX_DECREMENT
            encode('-',  '=', 0x0, spaced), // 0x18 SPACED_MINUS_EQUALS
            encode('-',  '>', 0x0, joined), // 0x19 JOINED_SINGLE_ARROW
            encode('-',  0x0, 0x0, spaced), // 0x1a SPACED_MINUS
            encode('-',  0x0, 0x0, prefix), // 0x1b PREFIX_MINUS
            encode('.',  '.', '.', suffix), // 0x1c SUFFIX_TRIPLE_PERIOD
            encode('.',  '.', 0x0, joined), // 0x1d JOINED_DOUBLE_PERIOD
            encode('.',  0x0, 0x0, joined), // 0x1e JOINED_PERIOD
            encode('/',  '*', 0x0, ignore), // 0x1f LEFT_COMMENT_BRACKET
            encode('/',  '=', 0x0, spaced), // 0x20 SPACED_DIVIDE_EQUALS
            encode('/',  0x0, 0x0, spaced), // 0x21 SPACED_SLASH
            encode(':',  ':', 0x0, joined), // 0x22 JOINED_DOUBLE_COLON
            encode(':',  0x0, 0x0, accord), // 0x23 COLON
            encode(';',  0x0, 0x0, ignore), // 0x24 SEMICOLON
            encode('<',  '|', '=', spaced), // 0x25 SPACED_SATURATING_LEFT_SHIFT_EQUALS
            encode('<',  '<', '=', spaced), // 0x26 SPACED_MODULAR_LEFT_SHIFT_EQUALS
            encode('<',  '<', 0x0, spaced), // 0x27 SPACED_MODULAR_LEFT_SHIFT
            encode('<',  '=', 0x0, spaced), // 0x28 SPACED_COMPARE_LE
            encode('<',  '|', 0x0, spaced), // 0x29 SPACED_SATURATING_LEFT_SHIFT
            encode('<',  0x0, 0x0, spaced), // 0x2a SPACED_COMPARE_LT
            encode('<',  0x0, 0x0, attach), // 0x2b LEFT_TEMPLATE_BRACKET
            encode('=',  '=', 0x0, spaced), // 0x2c SPACED_COMPARE_EQ
            encode('=',  '>', 0x0, spaced), // 0x2d SPACED_DOUBLE_ARROW
            encode('=',  0x0, 0x0, spaced), // 0x2e SPACED_EQUALS
            encode('>',  '>', '=', spaced), // 0x2f SPACED_MODULAR_RIGHT_SHIFT_EQUALS
            encode('>',  '=', 0x0, spaced), // 0x30 SPACED_COMPARE_GE
            encode('>',  '>', 0x0, spaced), // 0x31 SPACED_MODULAR_RIGHT_SHIFT
            encode('>',  0x0, 0x0, spaced), // 0x32 SPACED_COMPARE_GT
            encode('>',  0x0, 0x0, attach), // 0x33 RIGHT_TEMPLATE_BRACKET
            encode('@',  0x0, 0x0, ignore), // 0x34 AT_SIGN
            encode('[',  0x0, 0x0, ignore), // 0x35 LEFT_SQUARE_BRACKET
            encode(']',  0x0, 0x0, ignore), // 0x36 RIGHT_SQUARE_BRACKET
            encode('^',  '^', '=', spaced), // 0x37 SPACED_LOGICAL_XOR_EQUALS
            encode('^',  '=', 0x0, spaced), // 0x38 SPACED_BITWISE_XOR_EQUALS
            encode('^',  '^', 0x0, spaced), // 0x39 SPACED_LOGICAL_XOR
            encode('^',  0x0, 0x0, spaced), // 0x3a SPACED_CARET
            encode('^',  0x0, 0x0, suffix), // 0x3b SUFFIX_CARET
            encode('{',  0x0, 0x0, ignore), // 0x3c LEFT_CURLY_BRACKET
            encode('|',  '>', '=', spaced), // 0x3d SPACED_SATURATING_RIGHT_SHIFT_EQUALS
            encode('|',  '|', '=', spaced), // 0x3e SPACED_LOGICAL_OR_EQUALS
            encode('|',  '=', 0x0, spaced), // 0x3f SPACED_BITWISE_OR_EQUALS
            encode('|',  '|', 0x0, spaced), // 0x40 SPACED_LOGICAL_OR
            encode('|',  '>', 0x0, spaced), // 0x41 SPACED_SATURATING_RIGHT_SHIFT
            encode('|',  0x0, 0x0, spaced), // 0x42 SPACED_PIPE
            encode('}',  0x0, 0x0, ignore), // 0x43 RIGHT_CURLY_BRACKET
            encode('~',  0x0, 0x0, prefix)  // 0x44 PREFIX_TILDE
        };

        if (('0' <= codepoints[1]) && (codepoints[1] <= '9')) {
            return Token_type::NUMERIC_LITERAL;
        }

        if (
            (('a' <= codepoints[1]) && ( codepoints[1] <= 'z')) ||
            (('A' <= codepoints[1]) && ( codepoints[1] <= 'Z')) ||
            codepoints[1] == '_'
        ) {
            return Token_type::TEXT;
        }

        Token_type ret =  Token_type::NULL_TOKEN;

        for (std::size_t i = 1; i < token_table.size(); ++i) {
            std::uint8_t cdpt0 = token_table[i][0] & 0x7f;
            std::uint8_t cdpt1 = token_table[i][1] & 0x7f;
            std::uint8_t cdpt2 = token_table[i][2] & 0x7f;

            bool codepoint_match =
                (cdpt0 == 0x0) | (cdpt0 == codepoints[1]) &
                (cdpt1 == 0x0) | (cdpt1 == codepoints[1]) &
                (cdpt2 == 0x0) | (cdpt2 == codepoints[1]);

            int end_index = 0;

            bool x = codepoint_categories[0];
            bool y = codepoint_categories[end_index];

            bool a = token_table[i][0] >> 7;
            bool b = token_table[i][1] >> 7;
            bool c = token_table[i][2] >> 7;

            bool whitespace_match = test_whitespace(a, b, c, x, y);

            if (codepoint_match && whitespace_match) {
                ret = static_cast<Token_type>(i);
                break;
            }
        }

        //TODO: Consider alternatives:
        // Binary search
        // SIMD vectorized search
        return ret;
    }

    void find_single_line_comment_end(
        std::array<UChar32, 5>& context,
        std::bitset<5>& context_category,
        icu::StringCharacterIterator& it
    ) {
        // context[0] is irrelevant
        // context[1] is /
        // context[2] is /
        // context[3] is unknown
        // context[4] is unknown

        // Iterate until z is a newline
        for (; context[1] != icu::StringCharacterIterator::DONE; context[1] = it.next32PostInc()) {
            if (context[1] == '\n') {
                break;
            }
        }

        // Perform one shift left of chars to ensure context[1] is one past the new line
        context[0] = context[1];
        context[1] = context[2];
        context[2] = context[3];
        context[3] = context[4];
        context[4] = it.next32PostInc();

        // Recompute whitespace categorization
        context_category[0] = context[1] == '\n';
        for (int i = 1; i < 5; ++i) {
            context_category[i] = u_isWhitespace(context[i]);
        }
    }

    void find_numeric_literal_end(
        UChar32& x,
        UChar32& y,
        UChar32& z,
        icu::StringCharacterIterator& it
    ) {
        // x is a digit
        // y is unknown
        // z is unknown

        if (y == icu::StringCharacterIterator::DONE || !(u_isdigit(y) || u_isalpha(y) || y == '_' || y == '.')) {
            return;
        }

        while (z != icu::StringCharacterIterator::DONE) {
            if (!(u_isdigit(z) || u_isalpha(z) || z == '_' || z == '.')) {
                break;
            }

            z = it.next32PostInc();
        }

        y = z;
        z = it.next32PostInc();
    }

    void find_string_literal_end(UChar32& x, UChar32& y, UChar32& z, icu::StringCharacterIterator& it) {
        // x is "
        // y is unknown
        // z is unknown

        if (y == icu::StringCharacterIterator::DONE || y == '"' || y == '\n') {
            y = z;
            z = it.next32PostInc();

            return;
        }

        bool is_codepoint_escaped = y == '\\';

        while (z != icu::StringCharacterIterator::DONE) {
            if ((z == '"' && !is_codepoint_escaped) || y == '\n') {
                break;
            }

            is_codepoint_escaped = (z == '\\') && !is_codepoint_escaped;

            z = it.next32PostInc();
        }

        y = z;
        z = it.next32PostInc();

        y = z;
        z = it.next32PostInc();
    }

    void find_char_literal_end(UChar32& x, UChar32& y, UChar32& z, icu::StringCharacterIterator& it) {
        // x is '
        // y is unknown
        // z is unknown

        if (y == icu::StringCharacterIterator::DONE || y == '\'') {
            y = z;
            z = it.next32PostInc();

            return;
        }

        bool is_codepoint_escaped = y == '\\';

        while (z != icu::StringCharacterIterator::DONE) {
            if (z == '\'' && !is_codepoint_escaped) {
                break;
            }

            is_codepoint_escaped = (z == '\\') && !is_codepoint_escaped;

            z = it.next32PostInc();
        }

        y = z;
        z = it.next32PostInc();

        y = z;
        z = it.next32PostInc();
    }

    void find_text_token_end(UChar32& x, UChar32& y, UChar32& z, icu::StringCharacterIterator& it) {
        // x is codepoint which may begin identifier
        // y is unknown
        // z is unknown

        if (y == icu::StringCharacterIterator::DONE || !u_hasBinaryProperty(y, UProperty::UCHAR_ID_CONTINUE)) {
            return;
        }

        while (z != icu::StringCharacterIterator::DONE) {
            if (!u_hasBinaryProperty(z, UProperty::UCHAR_ID_CONTINUE)) {
                break;
            }

            z = it.next32PostInc();
        }

        y = z;
        z = it.next32PostInc();
    }

    void find_doc_text_end(UChar32& x, UChar32& y, UChar32& z, icu::StringCharacterIterator& it) {
        // x is /
        // y is /
        // z is /

        while (z != icu::StringCharacterIterator::DONE) {
            if (z == '\n') {
                break;
            }

            z = it.next32PostInc();
        }

        y = z;
        z = it.next32PostInc();
    }

    std::uint32_t estimate_bytes_per_token(std::string_view source) {
        const std::uint32_t bytes_per_token_estimate = 5;
        auto token_count_estimate = source.size() / bytes_per_token_estimate;

        return token_count_estimate;
    }

    std::uint32_t estimate_line_count(std::string_view source) {
        const std::uint32_t bytes_per_line_estimate = 8;
        auto line_count_estimate = source.size() / bytes_per_line_estimate;

        return line_count_estimate;
    }

    //=========================================================================
    // Tokenization subroutine
    //=========================================================================

    struct Stack_entry {
        Token_type type;
        std::uint32_t index;
    };

     */


    Tokenization lex(std::string_view source_id, std::string_view source, const Config& config) {
        Tokenizer tokenizer{source_id, source, config};
        return tokenizer.tokenize();
    }

    /*
    Tokenization lex(std::string_view source_id, std::string_view source, const Config& config) {
        Tokenizer tokenizer{source_id, source, config};
        return tokenizer.tokenize();

        //TODO: Add hot/cold path for ASCII/UTF-8 data
        //TODO: Remove use of ICU library


        Tokenization ret;
        ret.source = source;

        // Reserve space for data upfront
        std::uint32_t token_count_estimate = estimate_bytes_per_token(source);
        ret.types.reserve(token_count_estimate);
        ret.lengths.reserve(token_count_estimate);
        ret.source_indices.reserve(token_count_estimate);

        // Guess number of lines and reserve enough space
        std::uint32_t line_count_estimate = estimate_line_count(source);
        ret.line_indices.reserve(line_count_estimate);
        ret.line_indices.push_back(0);


        // Helper lambda to make emplacing tokens more terse
        auto emplace_token =
            [&ret] (
                Token_type type,
                std::uint32_t index,
                std::uint32_t length,
                std::array<std::uint32_t, 2> location,
                std::uint32_t pair_index = UINT32_MAX
            ) {
                ret.types.push_back(type);
                ret.source_indices.push_back(index);
                ret.lengths.push_back(length);

                if (pair_index == UINT32_MAX) {
                    ret.pair_indices.push_back(index);
                } else {
                    ret.pair_indices.push_back(pair_index);
                }
            };


        //TODO: Replace use of ICU library with internal UTF-8 handling
        // The current approach does not meet Harmonia's requirement that
        // source files may be up to at least 2^32-1 bytes in size
        icu::UnicodeString ustring{source.data(), static_cast<std::int32_t>(source.size()), "utf-8"};

        // Append two more empty characters so that getting the indices from the
        // iterator continue to be incremented even after reaching the end of
        // the string.
        ustring += eof_codepoint;
        ustring += eof_codepoint;
        ustring += eof_codepoint;
        ustring += eof_codepoint;

        // Index of codepoint which begins current line
        std::uint32_t line_begin_index = 0;

        // Track current line number
        std::uint32_t line = 1;

        // Codepoint iterator
        icu::StringCharacterIterator it{ustring};

        // Stack which is used to identify unpaired tokens
        std::vector<Stack_entry> balancing_stack;

        // Guess maximum nesting depth
        balancing_stack.reserve(32);

        // Location of current codepoint
        //std::array<std::uint32_t, 2> location = {line, it.getIndex() - 2 - line_begin_index};

        // Stores windows of relevant codepoints. Index 1 is considered the current codepoint
        std::array<UChar32, 5> context{
            0x00,
            it.next32PostInc(),
            it.next32PostInc(),
            it.next32PostInc(),
            it.next32PostInc()
        };

        // Indicates whether the corresponding entry in context is a whitespace codepoint or not
        std::bitset<5> context_category{};
        context_category[0] = true; // Treat start of file as whitespace
        context_category[1] = (u_isWhitespace(context[1]) != 0) | (context[1] == eof_codepoint);
        context_category[2] = (u_isWhitespace(context[2]) != 0) | (context[2] == eof_codepoint);
        context_category[3] = (u_isWhitespace(context[3]) != 0) | (context[3] == eof_codepoint);
        context_category[4] = (u_isWhitespace(context[4]) != 0) | (context[4] == eof_codepoint);

        // Lambda useful for moving window forward.
        // o is the minimum number of codepoint to advance by.
        // The advance will also cover any whitespace which may come thereafter.
        auto advance_window = [&] (unsigned o = 1) {
            // Skip forward the indicated amount
            std::shift_left(context.begin(), context.end(), o);
            context_category >>= o;

            for (std::size_t i = 5 - o; i < 5; ++i) {
                context[i] = it.next32PostInc();
                context_category[i] = u_isWhitespace(context[i]) || (context[i] == eof_codepoint);
            }

            // Skip whitespace
            while (context_category[1] && (context[1] != eof_codepoint)) {
                std::shift_left(context.begin(), context.end(), 1);
                context_category >>= 1;

                context[4] = it.next32PostInc();
                context_category[4] = u_isWhitespace(context[4]) || (context[4] == eof_codepoint);

                // Special handling for newlines
                if (context[1] == '\n') {
                    line += 1;
                    line_begin_index = it.getIndex() - 4;
                    ret.line_indices.push_back(line_begin_index);
                }
            }
        };

        while (context[1] != eof_codepoint) {
            // Identify type of current token
            Token_type type = identify_token(context, context_category);

            // Handle case where token was not recognized
            if (type == Token_type::NULL_TOKEN) {
                ret.message_buffer.error(
                    "Unrecognized token type",
                    source_id,
                    line,
                    it.getIndex() - 2 - line_begin_index
                );
                continue;
            }

            // Handle single line comment
            if (type == Token_type::SINGLE_LINE_COMMENT) {
                find_single_line_comment_end(context, context_category, it);
                continue;
            }

            // Handle multiline comment
            if (type == Token_type::LEFT_COMMENT_BRACKET) {
                //TODO: Update implementation to handle comment brackets
                //TODO: Consider ignoring template brackets which are found
                // within string literals

                balancing_stack.push_back(
                    Stack_entry{
                        Token_type::LEFT_COMMENT_BRACKET,
                        static_cast<std::uint32_t>(it.getIndex())
                    }
                );

                std::shift_left(context.begin(), context.end(), 1);
                context[4] = it.next32PostInc();

                while (context[1] != eof_codepoint) {
                    if (context[1] == '\n') {
                        line += 1;
                        line_begin_index = it.getIndex() - 2;
                        ret.line_indices.push_back(line_begin_index);
                    }

                    if (context[1] == '*' && context[2] == '/') {
                        if (balancing_stack.size() == 1) {
                            balancing_stack.pop_back();
                            break;
                        }
                    } else if (context[1] == '/' && context[2] == '*') {
                        balancing_stack.push_back(
                            Stack_entry{
                                Token_type::LEFT_COMMENT_BRACKET,
                                static_cast<std::uint32_t>(it.getIndex())
                            }
                        );
                    }
                }
                continue;
            }

            // Handle textural tokens, whose lengths are variable
            if (is_textual(type)) {
                auto starting_index = it.getIndex() - 3;
                switch (type) {
                    case Token_type::NUMERIC_LITERAL: {
                        find_numeric_literal_end(x, y, z, it);
                        break;
                    }
                    case Token_type::STRING_LITERAL: {
                        find_string_literal_end(x, y, z, it);
                        if (y == icu::StringCharacterIterator::DONE) {
                            ret.message_buffer.error(
                                "Unmatched double quotes",
                                source_id,
                                line,
                                it.getIndex() - line_begin_index - 1
                            );
                            continue;
                        }
                        break;
                    }
                    case Token_type::CHAR_LITERAL: {
                        find_char_literal_end(x, y, z, it);
                        if (y == icu::StringCharacterIterator::DONE) {
                            ret.message_buffer.error(
                                "Unmatched single quote",
                                source_id,
                                line,
                                it.getIndex() - line_begin_index - 1
                            );
                            continue;
                        }

                        break;
                    }
                    case Token_type::TEXT: {
                        find_text_token_end(x, y, z, it);

                        // Reset the balancing stack if the text token
                        // introduces a new function or struct
                        auto token_text = std::string_view{
                            source.data() + starting_index,
                            std::size_t(it.getIndex() - starting_index - 2)
                        };

                        if (token_text == "struct" || token_text == "func") {
                            if (!balancing_stack.empty()) {
                                //TODO: Handle lack of properly balanced tokens
                                balancing_stack.clear();
                            }
                        }

                        break;
                    }
                    case Token_type::DOC_TEXT: {
                        find_doc_text_end(x, y, z, it);
                        break;
                    }
                    default: {
                        // TODO: Report internal compiler error
                        return ret;
                    }
                }

                auto curr_index = it.getIndex();
                emplace_token(
                    type,
                    starting_index,
                    curr_index - starting_index - 2,
                    {line, starting_index - line_begin_index + 1}
                );

                continue;
            }

            // Handle non-textual tokens, whose lengths are at most 3 code points
            auto width = token_width(type);


            // Handle balancing tokens
            auto expected_match = matching_token_type(type);

            //TODO: Handle mismatched tokens and attempt to identify which
            // those are.

            std::uint32_t matching_index = UINT32_MAX;
            if (expected_match != Token_type::NULL_TOKEN) {
                if (is_left_token(type)) {
                    Stack_entry b{};
                    b.type = type;
                    b.index = ret.lengths.size();
                    balancing_stack.push_back(b);
                } else {
                    auto stack_top = balancing_stack.back();
                    if (expected_match == stack_top.type) {
                        balancing_stack.pop_back();
                        matching_index = stack_top.index;

                        ret.pair_indices[stack_top.index] = ret.lengths.size();
                    } else {
                        std::string_view error_message;
                        switch (stack_top.type) {
                            case Token_type::LEFT_PARENTHESIS:      error_message = "Unpaired parenthesis"; break;
                            case Token_type::LEFT_SQUARE_BRACKET:   error_message = "Unpaired square bracket"; break;
                            case Token_type::LEFT_CURLY_BRACKET:    error_message = "Unpaired curly bracket"; break;
                            case Token_type::LEFT_TEMPLATE_BRACKET: error_message = "Unpaired template bracket"; break;
                            default: {
                                //TODO: Report internal compiler error
                            }
                        }

                        ret.message_buffer.error(
                            error_message,
                            source_id,
                            line,
                            it.getIndex() + 1
                        );
                    }
                }
            }

            emplace_token(
                type,
                it.getIndex() - 3,
                width,
                location,
                matching_index
            );

            // Move context window left
            advance_window(width);
        }

        return ret;
    }
    */

}
