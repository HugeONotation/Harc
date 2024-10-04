#include <cstdint>

#include <iostream>
#include <fstream>

#include<array>
#include <string_view>
#include <string>

#include <vector>
#include <algorithm>
#include <unordered_map>

namespace unicode_properties {

    enum class Codepoint_category {
        UNRECOGNIZED = 0,

        IDENTIFIER_START = 1,
        IDENTIFIER_CONTINUE = 2,
        NEWLINE = 3,
        OTHER_WHITESPACE = 4,

        EXCLAMATION = '!',
        DOUBLE_QUOTES = '"',
        SINGLE_QUOTE = '\'',
        CARET = '^',
        AMPERSAND = '&',
        PIPE = '|',
        TILDE = '~',
        COLON = ':',
        SEMICOLON = ';',
        COMMA = ',',

        PLUS = '+',
        MINUS = '-',
        ASTERISK = '*',
        FORWARD_SLASH = '/',
        PERCENT_SIGN = '%',

        BACKSLASH = '\\',

        EQUAL_SIGN = '=',

        L_PAREN = '(',
        R_PAREN = ')',
        L_CURLY_BRACKET  = '{',
        R_CURLY_BRACKET  = '}',
        L_SQUARE_BRACKET = '[',
        R_SQUARE_BRACKET = ']',
        L_ANGLE_BRACKET  = '<',
        R_ANGLE_BRACKET  = '>',

        DOT = '.'
    };

    [[nodiscard]]
    std::string_view to_string(Codepoint_category c) {
        switch (c) {
            case Codepoint_category::IDENTIFIER_START:
                return "Identifier start";
            case Codepoint_category::IDENTIFIER_CONTINUE:
                return "Identifier continue";
            case Codepoint_category::NEWLINE:
                return "New line";
            case Codepoint_category::OTHER_WHITESPACE:
                return "White space";
            case Codepoint_category::EXCLAMATION:
                return "Exclamation";
            case Codepoint_category::DOUBLE_QUOTES:
                return "Double quotes";
            case Codepoint_category::SINGLE_QUOTE:
                return "Single quotes";
            case Codepoint_category::CARET:
                return "Caret";
            case Codepoint_category::AMPERSAND:
                return "Ampersand";
            case Codepoint_category::PIPE:
                return "Pipe";
            case Codepoint_category::TILDE:
                return "Tilde";
            case Codepoint_category::COLON:
                return "Colon";
            case Codepoint_category::SEMICOLON:
                return "Semicolon";
            case Codepoint_category::COMMA:
                return "Comma";
            case Codepoint_category::PLUS:
                return "Plus";
            case Codepoint_category::MINUS:
                return "Minus";
            case Codepoint_category::ASTERISK:
                return "Asterisk";
            case Codepoint_category::FORWARD_SLASH:
                return "Forward slash";
            case Codepoint_category::PERCENT_SIGN:
                return "Percent sign";
            case Codepoint_category::BACKSLASH:
                return "Backslash";
            case Codepoint_category::EQUAL_SIGN:
                return "Equal sign";
            case Codepoint_category::L_PAREN:
                return "Left parenthesis";
            case Codepoint_category::R_PAREN:
                return "Right parenthesis";
            case Codepoint_category::L_CURLY_BRACKET:
                return "Left curly bracket";
            case Codepoint_category::R_CURLY_BRACKET:
                return "Right curly bracket";
            case Codepoint_category::L_SQUARE_BRACKET:
                return "Left square bracket";
            case Codepoint_category::R_SQUARE_BRACKET:
                return "Right square bracket";
            case Codepoint_category::L_ANGLE_BRACKET:
                return "Left angle bracket";
            case Codepoint_category::R_ANGLE_BRACKET:
                return "Right angle bracket";
            case Codepoint_category::DOT:
                return "DOT";
            default:
                return "Unrecognized";
        }
    }

    std::vector<std::string_view> split(
        std::string_view str,
        std::string_view delimiter
    ) {
        std::vector<std::string_view> ret;

        std::size_t pos_start = 0;
        std::size_t pos_end;
        std::size_t delim_len = delimiter.length();
        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            std::string_view token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            if (!token.empty()) {
                ret.push_back(token);
            }
        }

        ret.push_back(str.substr(pos_start));
        return ret;
    }

    ///
    /// \param str ASCII-encoded string
    /// \return Copy if input string with leading and trailing whitespace removed
    [[nodiscard]]
    std::string trim_whitespace(const std::string& str) {
        auto it0 = str.begin();
        while (std::isspace(*it0)) {
            ++it0;
        }

        if (std::string{it0, str.end()}.empty()) {
            return {};
        }

        auto it1 = str.end();
        do {
            --it1;
        } while (std::isspace(*it1));
        ++it1;

        return std::string{it0, it1};
    }

    enum class Queried_property {
        GRAPHEME_BREAK,
        GENERAL_CATEGORY
    };

    struct Arguments {
        std::string_view ucd_path;
    };

    Arguments arguments;

    void parse_command_line_arguments(int argc, char* argv[]) {
        //TODO: Implement
    }

    enum class Grapheme_cluster_break : std::uint8_t {
        NULL_GRAPHEME_CLUSTER_BREAK,
        PREPEND,
        CARRIAGE_RETURN,
        LINE_FEED,
        CONTROL,
        EXTEND,
        SPACING_MARK,
        HANGUL_LEADING_CONSTANT,
        HANGUL_VOWEL,
        HANGUL_TRAILING_CONSTANT,
        HANGUL_LV_SEQUENCE,
        HANGUL_LVT_SEQUENCE,
        ZERO_WIDTH_JOINER,
        OTHER
    };

    [[nodiscard]]
    std::string_view to_string(Grapheme_cluster_break extend) {
        switch (extend) {
            case Grapheme_cluster_break::PREPEND:
                return "Prepend";
            case Grapheme_cluster_break::CARRIAGE_RETURN:
                return "CR";
            case Grapheme_cluster_break::LINE_FEED:
                return "LF";
            case Grapheme_cluster_break::CONTROL:
                return "Control";
            case Grapheme_cluster_break::EXTEND:
                return "Extend";
            case Grapheme_cluster_break::SPACING_MARK:
                return "Spacing mark";
            case Grapheme_cluster_break::HANGUL_LEADING_CONSTANT:
                return "Hangul leading constant";
            case Grapheme_cluster_break::HANGUL_VOWEL:
                return "Hangul vowel";
            case Grapheme_cluster_break::HANGUL_TRAILING_CONSTANT:
                return "hangul trailing_constant";
            case Grapheme_cluster_break::HANGUL_LV_SEQUENCE:
                return "hangul LV sequence";
            case Grapheme_cluster_break::HANGUL_LVT_SEQUENCE:
                return "hangul LVT sequence";
            case Grapheme_cluster_break::ZERO_WIDTH_JOINER:
                return "zero-width joiner";
            case Grapheme_cluster_break::OTHER:
                return "other";
            default:
                return "Unrecognized Grapheme_extend enum value";
        }
    }

    [[nodiscard]]
    Grapheme_cluster_break string_to_grapheme_cluster_break(std::string_view str) {
        static std::unordered_map<std::string_view, Grapheme_cluster_break> mappings{
            {"Prepend",     Grapheme_cluster_break::PREPEND},
            {"CR",          Grapheme_cluster_break::CARRIAGE_RETURN},
            {"LF",          Grapheme_cluster_break::LINE_FEED},
            {"Control",     Grapheme_cluster_break::CONTROL},
            {"Extend",      Grapheme_cluster_break::EXTEND},
            {"SpacingMark", Grapheme_cluster_break::SPACING_MARK},
            {"L",           Grapheme_cluster_break::HANGUL_LEADING_CONSTANT},
            {"V",           Grapheme_cluster_break::HANGUL_VOWEL},
            {"T",           Grapheme_cluster_break::HANGUL_TRAILING_CONSTANT},
            {"LV",          Grapheme_cluster_break::HANGUL_LV_SEQUENCE},
            {"LVT",         Grapheme_cluster_break::HANGUL_LVT_SEQUENCE},
            {"ZWJ",         Grapheme_cluster_break::ZERO_WIDTH_JOINER}
        };

        auto it = mappings.find(str);
        if (it == mappings.end()) {
            return Grapheme_cluster_break::NULL_GRAPHEME_CLUSTER_BREAK;
        } else {
            return std::get<1>(*it);
        }
    }

    enum class General_category : std::uint8_t {
        UNCATEGORIZED,
        UPPERCASE_LETTER,
        LOWERCASE_LETTER,
        TITLECASE_LETTER,
        MODIFIER_LETTER,
        OTHER_LETTER,
        NONSPACING_MARK,
        ENCLOSING_MARK,
        SPACING_MARK,
        DECIMAL_NUMBER,
        LETTER_NUMBER,
        OTHER_NUMBER,
        SPACE_SEPARATOR,
        LINE_SEPARATOR,
        PARAGRAPH_SEPARATOR,
        CONTROL,
        PRIVATE_USE,
        SURROGATE,
        DASH_PUNCTUATION,
        OPEN_PUNCTUATION,
        CLOSE_PUNCTUATION,
        CONNECTOR_PUNCTUATION,
        OTHER_PUNCTUATION,
        MATH_SYMBOL,
        CURRENCY_SYMBOL,
        MODIFIER_SYMBOL,
        OTHER_SYMBOL,
        INITIAL_PUNCTUATION,
        FINAL_PUNCTUATION
    };

    [[nodiscard]]
    std::vector<Grapheme_cluster_break> parse_property(std::string_view path) {
        std::ifstream fin{path.data()};

        std::vector<Grapheme_cluster_break> results;
        results.resize(1 << 21, Grapheme_cluster_break::OTHER);

        std::string line;
        while (std::getline(fin, line)) {
            // Trim off commented portion of line, if it exists
            line.erase(std::find(line.begin(), line.end(), '#'), line.end());

            // Separate lines into codepoint ranges and GraphemeBreakProperty categories
            auto semicolon_it = std::find(line.begin(), line.end(), ';');

            std::string range_string{line.begin(), semicolon_it};
            range_string = trim_whitespace(range_string);

            std::string category_string{semicolon_it + 1, line.end()};
            category_string = trim_whitespace(category_string);



            // Compute range bounds (inclusive)
            std::int32_t range_begin = 0;
            std::int32_t range_end = 0;

            auto delimiter_pos = range_string.find("..");
            if (delimiter_pos != std::string::npos) {
                std::string range_begin_str{range_string.begin(), range_string.begin() + delimiter_pos};
                range_begin = std::stoi(range_begin_str, nullptr, 16);

                std::string range_end_str  {range_string.begin() + delimiter_pos + 2, range_string.end()};
                range_end = std::stoi(range_end_str, nullptr, 16);
            } else {
                range_begin = std::stoi(range_string);
                range_end = range_end;
            }

            // Parse category
            auto category = string_to_grapheme_cluster_break(category_string);

            for (std::int32_t i = range_begin; i <= range_end; ++i) {
                results[i] = category;
            }
        }

        return results;
    }

    enum class Derived_core_property : std::uint8_t {
        NULL_DERIVED_CORE_PROPERTY,
        ALPHABETIC,
        CASE_IGNORABLE,
        CASED,
        CHANGES_WHEN_CASEFOLDED,
        CHANGES_WHEN_CASEMAPPED,
        CHANGES_WHEN_LOWERCASED,
        CHANGES_WHEN_TITLECASED,
        CHANGES_WHEN_UPPERCASED,
        DEFAULT_IGNORABLE_CODE_POINT,
        GRAPHEME_BASE,
        GRAPHEME_EXTEND,
        GRAPHEME_LINK,
        ID_CONTINUE,
        ID_START,
        INDIC_CONJUNCT_BREAK,
        LOWERCASE,
        MATH,
        UPPERCASE,
        XID_CONTINUE,
        XID_START
    };

    [[nodiscard]]
    std::string_view to_string(Derived_core_property p) {
        constexpr std::array<std::string_view, 21> table {
            "NULL_CATEGORY",
            "Alphabetic",
            "Case_Ignorable",
            "Cased",
            "Changes_When_Casefolded",
            "Changes_When_Casemapped",
            "Changes_When_Lowercased",
            "Changes_When_Titlecased",
            "Changes_When_Uppercased",
            "Default_Ignorable_Code_Point",
            "Grapheme_Base",
            "Grapheme_Extend",
            "Grapheme_Link",
            "ID_Continue",
            "ID_Start",
            "Indic_Conjunct_Break",
            "Lowercase",
            "Math",
            "Uppercase",
            "XID_Continue",
            "XID_Start"
        };

        return table[int(p)];
    }

    [[nodiscard]]
    Derived_core_property parse_derive_core_property(std::string_view str) {
        constexpr std::array<std::string_view, 20> table {
            "Alphabetic",
            "Case_Ignorable",
            "Cased",
            "Changes_When_Casefolded",
            "Changes_When_Casemapped",
            "Changes_When_Lowercased",
            "Changes_When_Titlecased",
            "Changes_When_Uppercased",
            "Default_Ignorable_Code_Point",
            "Grapheme_Base",
            "Grapheme_Extend",
            "Grapheme_Link",
            "ID_Continue",
            "ID_Start",
            "Indic_Conjunct_Break",
            "Lowercase",
            "Math",
            "Uppercase",
            "XID_Continue",
            "XID_Start"
        };

        auto* p = std::lower_bound(table.begin(), table.end(), str);
        if (p == table.end() || *p != str) {
            return Derived_core_property::NULL_DERIVED_CORE_PROPERTY;
        } else {
            auto val = p - table.begin() + 1;
            return Derived_core_property(val);
        }
    }

    std::vector<bool> derived_core_property_set(std::string_view path, Derived_core_property p) {
        std::ifstream fin{path.data()};

        std::vector<bool> results;
        results.resize(1 << 21, false);

        std::string line;
        while (std::getline(fin, line)) {
            // Trim off commented portion of line, if it exists
            line.erase(std::find(line.begin(), line.end(), '#'), line.end());

            // Separate lines into codepoint ranges and GraphemeBreakProperty categories
            auto semicolon_it = std::find(line.begin(), line.end(), ';');
            if (semicolon_it == line.end()) {
                continue;
            }

            std::string range_string{line.begin(), semicolon_it};
            range_string = trim_whitespace(range_string);

            std::string category_string{semicolon_it + 1, line.end()};
            category_string = trim_whitespace(category_string);



            // Compute range bounds (inclusive)
            std::int32_t range_first = 0;
            std::int32_t range_last = 0;

            auto delimiter_pos = range_string.find("..");
            if (delimiter_pos != std::string::npos) {
                std::string range_begin_str{range_string.begin(), range_string.begin() + delimiter_pos};
                range_first = std::stoi(range_begin_str, nullptr, 16);

                std::string range_end_str  {range_string.begin() + delimiter_pos + 2, range_string.end()};
                range_last = std::stoi(range_end_str, nullptr, 16);
            } else {
                range_first = std::stoi(range_string, nullptr, 16);
                range_last = range_first;
            }

            // Parse property
            auto property = parse_derive_core_property(trim_whitespace(category_string));
            if (property == p) {
                for (std::int32_t i = range_first; i <= range_last; ++i) {
                    results[i] = true;
                }
            }
        }


        return results;
    }

    enum class Case {
        NULL_CASE,
        LOWERCASE,
        UPPERCASE
    };

    [[nodiscard]]
    std::string_view to_string(Case c) {
        switch (c) {
            case Case::NULL_CASE: return "Null case";
            case Case::LOWERCASE: return "Lowercase";
            case Case::UPPERCASE: return "Uppercase";
            default: return "Unrecognized";
        }
    }

    std::vector<Case> parse_case(std::string_view path) {
        std::vector<Case> results;
        results.resize(1 << 21, Case::NULL_CASE);

        std::ifstream fin{path.data()};
        std::string line_data;
        while (std::getline(fin, line_data)) {
            // Trim off commented portion of line, if it exists
            line_data.erase(std::find(line_data.begin(), line_data.end(), '#'), line_data.end());
            if (line_data.empty()) {
                continue;
            }

            std::string_view line = line_data;

            auto idx0 = line.find(';');
            std::string_view uppercase_codepoint_str = line.substr(0, idx0);

            auto idx1 = line.find(';', idx0 + 1);
            std::string_view case_status_str = line.substr(idx0 + 1, idx1 - (idx0 + 1));

            auto idx2 = line.find(';', idx1 + 1);
            std::string_view lowercase_codepoints_str = line.substr(idx1 + 1, idx2 - (idx1 + 1));

            std::uint32_t uppercase_codepoint = std::stoi(uppercase_codepoint_str.data(), nullptr, 16);
            results[uppercase_codepoint] = Case::UPPERCASE;

            std::vector<std::string_view> lowercase_codepoint_strs = split(lowercase_codepoints_str, " ");
            if (lowercase_codepoint_strs.size() == 1) {
                results[std::stoi(lowercase_codepoint_strs[0].data(), nullptr, 16)] = Case::LOWERCASE;
            }
        }

        return results;
    }

    template<class Enum>
    void print(const std::vector<Enum>& v) {
        for (std::size_t i = 0; i < v.size(); ++i) {
            std::cout << i << ". " << '\n';
        }
        std::cout.flush();
    }

    template<class Enum, Enum e>
    void print(const std::vector<Enum>& v) {
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (v[i] == e) {
                std::cout << i << '\n';
            }
        }
    }

    struct Codepoint_range {
        std::uint32_t first = 0;
        std::uint32_t last = 0;
    };

    template<class Enum>
    std::vector<Codepoint_range> coalesce(const std::vector<Enum>& v, Enum e) {
        std::vector<Codepoint_range> ranges;

        bool last_result = false;
        for (std::uint32_t i = 0; i < (1 << 21); ++i) {
            auto current_result = (v[i] == e);

            if (!last_result && current_result) {
                ranges.push_back(Codepoint_range{i, i});
            }

            if (last_result && current_result) {
                ranges.back().last = i;
            }

            last_result = current_result;
        }

        return ranges;
    }

    template<class Enum>
    std::vector<std::pair<Codepoint_range, Codepoint_category>>
    coalesce(const std::vector<Enum>& v) {
        std::vector<std::pair<Codepoint_range, Codepoint_category>> ranges;

        Enum last_value = v[0];
        ranges.emplace_back(Codepoint_range{0, 0}, last_value);
        for (std::uint32_t i = 1; i < (1 << 21); ++i) {
            auto current_result = v[i];

            if (last_value != current_result) {
                ranges.emplace_back(Codepoint_range{i, i}, current_result);
            } else {
                ranges.back().first.last = i;
            }

            last_value = current_result;
        }

        return ranges;
    }

    template<class Enum>
    void print(const std::vector<std::pair<Codepoint_range, Enum>>& list) {
        for (auto& p : list) {
            std::cout << '(' << p.first.first << ", " << p.first.last << ") " << to_string(p.second) << '\n';
        }
        std::cout.flush();
    }

    void print(const std::vector<Codepoint_range>& range_list) {
        for (auto& r : range_list) {
            std::cout << '(' << r.first << ", " << r.last << ')' << '\n';
        }
        std::cout.flush();
    }

    void print(const std::vector<Codepoint_category>& list) {
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (list[i] != Codepoint_category::UNRECOGNIZED) {
                std::cout << i << ":" << to_string(list[i]) << '\n';
            }
        }
        std::cout.flush();
    }

    ///
    /// \return A list containing the categorization of Unicode codepoints
    /// according to how they're recognized by the Harmonia programming language
    [[nodiscard]]
    std::vector<Codepoint_category> recognized_codepoints() {
        std::vector<Codepoint_category> categories{};
        categories.resize(1 << 21, Codepoint_category::UNRECOGNIZED);

        // Hard code recognized certain codepoints
        categories['!'] = Codepoint_category::EXCLAMATION;
        categories['"'] = Codepoint_category::DOUBLE_QUOTES;
        categories['%'] = Codepoint_category::PERCENT_SIGN;
        categories['&'] = Codepoint_category::AMPERSAND;
        categories['\''] = Codepoint_category::SINGLE_QUOTE;
        categories['('] = Codepoint_category::L_PAREN;
        categories[')'] = Codepoint_category::R_PAREN;
        categories['*'] = Codepoint_category::ASTERISK;
        categories['+'] = Codepoint_category::PLUS;
        categories[','] = Codepoint_category::COMMA;
        categories['-'] = Codepoint_category::MINUS;
        categories['.'] = Codepoint_category::DOT;
        categories['/'] = Codepoint_category::FORWARD_SLASH;
        categories[':'] = Codepoint_category::COLON;
        categories[';'] = Codepoint_category::SEMICOLON;
        categories['<'] = Codepoint_category::L_ANGLE_BRACKET;
        categories['='] = Codepoint_category::EQUAL_SIGN;
        categories['>'] = Codepoint_category::R_ANGLE_BRACKET;
        categories['['] = Codepoint_category::L_SQUARE_BRACKET;
        categories['\\'] = Codepoint_category::BACKSLASH;
        categories[']'] = Codepoint_category::R_SQUARE_BRACKET;
        categories['^'] = Codepoint_category::CARET;
        categories['{'] = Codepoint_category::L_CURLY_BRACKET;
        categories['|'] = Codepoint_category::PIPE;
        categories['}'] = Codepoint_category::R_CURLY_BRACKET;
        categories['~'] = Codepoint_category::TILDE;

        // Hard code whitespace characters for now
        /*
        categories[0x0009] = Codepoint_category::OTHER_WHITESPACE;
        categories['\n']   = Codepoint_category::NEWLINE;
        categories[0x000b] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x000c] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x000d] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x0020] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x0085] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x1680] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2000] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2001] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2002] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2003] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2004] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2005] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2006] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2007] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2008] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2009] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x200a] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2028] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2028] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x2029] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x202F] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x205F] = Codepoint_category::OTHER_WHITESPACE;
        categories[0x3000] = Codepoint_category::OTHER_WHITESPACE;
        */

        auto id_continue_list = derived_core_property_set("/home/avereniect/Downloads/UCD/DerivedCoreProperties.txt", Derived_core_property::ID_CONTINUE);
        for (std::size_t i = 0; i < id_continue_list.size(); ++i) {
            if (id_continue_list[i]) {
                categories[i] = Codepoint_category::IDENTIFIER_CONTINUE;
            }
        }

        auto id_start_list = derived_core_property_set("/home/avereniect/Downloads/UCD/DerivedCoreProperties.txt", Derived_core_property::ID_START);
        for (std::size_t i = 0; i < id_start_list.size(); ++i) {
            if (id_start_list[i]) {
                categories[i] = Codepoint_category::IDENTIFIER_START;
            }
        }

        return categories;
    }

}

int main(int argc, char* argv[]) {
    using namespace unicode_properties;

    auto categorizations = recognized_codepoints();

    auto category_ranges = coalesce(categorizations);

    auto end0 = std::remove_if(category_ranges.begin(), category_ranges.end(), [] (auto x) {return x.second == Codepoint_category::UNRECOGNIZED;});
    category_ranges = {category_ranges.begin(), end0};

    auto end1 = std::remove_if(category_ranges.begin(), category_ranges.end(), [] (auto x) {return x.first.first < 128;});
    category_ranges = {category_ranges.begin(), end1};

    auto end2 = std::remove_if(category_ranges.begin(), category_ranges.end(), [] (auto x) {return x.first.last > (1u << 15);});
    category_ranges = {category_ranges.begin(), end2};

    auto width = 8;
    for (std::size_t i = 0; i < category_ranges.size(); i += width) {
        for (std::size_t j = i; j < category_ranges.size() && j < i + width; ++j) {
            std::cout.width(6);
            std::cout << category_ranges[j].first.first << ", ";
        }
        std::cout << '\n';
    }

    for (std::size_t i = 0; i < category_ranges.size(); i += width) {
        for (std::size_t j = i; j < category_ranges.size() && j < i + width; ++j) {
            std::cout.width(6);
            std::cout << (category_ranges[j].first.last - category_ranges[j].first.first + 1) << ", ";
        }
        std::cout << '\n';
    }

    for (std::size_t i = 0; i < category_ranges.size(); i += 32) {
        std::uint32_t bits = 0;
        for (std::size_t j = i; j < i + 32 && j < category_ranges.size(); ++j) {
            bool t = category_ranges[j].second == Codepoint_category::IDENTIFIER_START;
            std::uint32_t d = (j % 32u);
            bits |= (std::uint32_t(t) << d);
        }

        std::cout.fill('0');
        std::cout << "0x";
        std::cout.width(8);
        std::cout << std::hex << bits << std::endl;
    }

    //print(tmp);

    //print(categorizations);
    //auto ranges = coalesce(categorizations, Codepoint_category::IDENTIFIER_CONTINUE);
    //print(ranges);

    //auto results = parse_case("/home/avereniect/Downloads/UCD/CaseFolding.txt");
    //auto ranges = coalesce(results, Case::UPPERCASE);
    //print(ranges);

    return EXIT_SUCCESS;
}
