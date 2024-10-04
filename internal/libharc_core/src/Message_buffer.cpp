#include <harc/Message_buffer.hpp>

#include <fmt/color.h>

#include <aul/containers/Array_map.hpp>

#include <cstring>
#include <iostream>

namespace harc {

    ///
    /// Message Markup tags represented as strings.
    ///
    inline constexpr std::string_view message_markup_tag_strings[] {
        "",

        "df",

        "ih",
        "ah",
        "wh",
        "eh",
    };

    ///
    /// Message Markup tags represented as strings, including unit separator
    /// character. Simplifies implementation of
    /// harc::Message_markup_builder::append.
    ///
    inline constexpr std::string_view message_markup_tag_strings_full[] {
        "",

        "\x1f" "df" "\x1f",

        "\x1f" "ih" "\x1f",
        "\x1f" "ah" "\x1f",
        "\x1f" "wh" "\x1f",
        "\x1f" "eh" "\x1f"
    };

    inline constexpr char unit_separator = 31;

    //=========================================================================
    // Message_markup_builder methods
    //=========================================================================

    void Message_markup_builder::append(
        std::string_view str,
        Message_markup_tag tag
    ) {

        if (tag == Message_markup_tag::NONE) {
            tag = Message_markup_tag::PLAIN;
        }

        std::size_t tag_overhead = (tag == last_tag) ? 0 : 2;
        std::string_view tag_text = message_markup_tag_strings[static_cast<int>(tag)];

        //TODO: Consider a single pass approach?

        // Count number of unit separators that occur in the text being pushed
        std::size_t unit_separator_count = 0;
        for (char c : str) {
            unit_separator_count += (c == unit_separator);
        }

        std::size_t reservation_size =
            buffer.size() +
            tag_overhead +
            tag_text.size() +
            unit_separator_count +
            str.size();

        buffer.reserve(reservation_size);

        // Print tag
        if (tag != last_tag) {
            buffer.push_back(unit_separator);
            buffer.append(tag_text);
            buffer.push_back(unit_separator);
        }

        // Print text
        if (unit_separator_count) [[unlikely]] {
            // Copy bytes one by one
            for (char c : str) {
                buffer.push_back(c);

                // Escape unit separator by printing two of them
                if (c == unit_separator) {
                    constexpr std::array<char, 2> escape_sequence{
                        unit_separator,
                        unit_separator
                    };

                    buffer.append(std::string_view{escape_sequence.begin(), escape_sequence.end()});
                }
            }
        } else {
            buffer.append(str);
        }
    }

    void Message_markup_builder::append(
        aul::Multispan<
            const std::string_view,
            const Message_markup_tag
        > strings_tags
    ) {
        auto strings = aul::get<0>(strings_tags);
        auto tags = aul::get<1>(strings_tags);

        // Make reservation upfront
        std::size_t reservation_size = buffer.size();

        for (auto tag : tags) {
            reservation_size += message_markup_tag_strings_full[int(tag)].size();
        }

        for (auto string : strings) {
            reservation_size += string.size();
        }

        buffer.reserve(reservation_size);

        // Push strings to buffer
        Message_markup_tag current_tag = last_tag;
        for (const auto& p : strings_tags) {
            auto string = std::get<0>(p);
            auto tag = std::get<1>(p);

            if (current_tag != tag) {
                auto idx = int(tag);
                buffer.append(message_markup_tag_strings_full[idx]);
            }

            std::size_t unit_separator_count = 0;
            for (char c : string) {
                unit_separator_count += (c == unit_separator);
            }

            if (unit_separator_count) {
                for (char c : string) {
                    //TODO: Optimize this
                    buffer.push_back(c);
                    if (c == 0x1f) {
                        buffer.push_back(c);
                    }
                }
            } else {
                buffer.append(string);
            }

        }
    }

    //=========================================================================
    // Message_buffer methods
    //=========================================================================

    //=====================================================
    // Message_buffer info methods
    //=====================================================

    constexpr std::string_view info_header_text = "INFO:\n";

    void Message_buffer::info(
        std::string_view message
    ) {
        const std::array<std::string_view, 3> strings{
            info_header_text,
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 3> tags{
            Message_markup_tag::INFO_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);
        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::info(
        std::string_view message,
        std::string_view path
    ) {
        const std::array<std::string_view, 5> strings{
            info_header_text,
            path,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 5> tags{
            Message_markup_tag::INFO_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);
        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::info(
        std::string_view message,
        std::string_view path,
        std::uint32_t line,
        std::uint32_t column
    ) {
        std::string line_number = std::to_string(line);
        std::string column_number = std::to_string(column);

        const std::array<std::string_view, 9> strings{
            info_header_text,
            path,
            ":",
            line_number,
            ":",
            column_number,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 9> tags{
            Message_markup_tag::INFO_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    //=====================================================
    // Message_buffer advice methods
    //=====================================================

    constexpr std::string_view advice_header_text = "ADVICE:\n";

    void Message_buffer::advice(
        std::string_view message
    ) {
        const std::array<std::string_view, 3> strings{
            advice_header_text,
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 3> tags{
            Message_markup_tag::ADVICE_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::advice(
        std::string_view message,
        std::string_view path
    ) {
        const std::array<std::string_view, 5> strings{
            advice_header_text,
            path,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 5> tags{
            Message_markup_tag::ADVICE_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::advice(
        std::string_view message,
        std::string_view path,
        std::uint32_t line,
        std::uint32_t column
    ) {
        std::string line_number = std::to_string(line);
        std::string column_number = std::to_string(column);

        const std::array<std::string_view, 9> strings{
            advice_header_text,
            path,
            ":",
            line_number,
            ":",
            column_number,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 9> tags{
            Message_markup_tag::ADVICE_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    //=====================================================
    // Message_buffer warning methods
    //=====================================================

    constexpr std::string_view warning_header_text = "WARNING:\n";

    void Message_buffer::warning(
        std::string_view message
    ) {
        const std::array<std::string_view, 3> strings{
            warning_header_text,
            message,
            "\n"
        };

        const std::array<Message_markup_tag, 3> tags{
            Message_markup_tag::WARNING_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::warning(
        std::string_view message,
        std::string_view path
    ) {
        const std::array<std::string_view, 5> strings{
            warning_header_text,
            path,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 5> tags{
            Message_markup_tag::WARNING_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::warning(
        std::string_view message,
        std::string_view path,
        std::uint32_t line,
        std::uint32_t column
    ) {
        std::string line_number = std::to_string(line);
        std::string column_number = std::to_string(column);

        const std::array<std::string_view, 9> strings{
            warning_header_text,
            path,
            ":",
            line_number,
            ":",
            column_number,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 9> tags{
            Message_markup_tag::WARNING_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    //=====================================================
    // Message_buffer error methods
    //=====================================================

    constexpr std::string_view error_header_text = "ERROR:\n";

    void Message_buffer::error(
        std::string_view message
    ) {
        const std::array<std::string_view, 3> strings{
            error_header_text,
            message,
            "\n"
        };

        const std::array<Message_markup_tag, 3> tags{
            Message_markup_tag::ERROR_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::error(
        std::string_view path,
        std::string_view message
    ) {
        const std::array<std::string_view, 5> strings{
            error_header_text,
            path,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 5> tags{
            Message_markup_tag::ERROR_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    void Message_buffer::error(
        std::string_view message,
        std::string_view path,
        std::uint32_t line,
        std::uint32_t column
    ) {
        std::string line_number = std::to_string(line);
        std::string column_number = std::to_string(column);

        const std::array<std::string_view, 9> strings{
            error_header_text,
            path,
            ":",
            line_number,
            ":",
            column_number,
            "\n",
            message,
            "\n\n"
        };

        const std::array<Message_markup_tag, 9> tags{
            Message_markup_tag::ERROR_HEADER,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,

            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN,
            Message_markup_tag::PLAIN
        };

        aul::Multispan<const std::string_view, const Message_markup_tag> mspan{
            strings.size(),
            strings.data(),
            tags.data()
        };

        Message_markup_builder builder;
        builder.append(mspan);

        auto t = builder.contents();
        buffer.insert(buffer.end(), t.begin(), t.end());
    }

    //=========================================================================
    // Misc. Message_buffer methods
    //=========================================================================

    void Message_buffer::append(const Message_buffer& other) {
        buffer.insert(buffer.end(), other.contents().begin(), other.contents().end());
    }

    void Message_buffer::append(Message_buffer&& other) {
        buffer.insert(buffer.end(), other.contents().begin(), other.contents().end());
    }

    //=========================================================================
    // Free functions
    //=========================================================================

    void print(
        const Message_buffer& message_buffer,
        const Print_style& print_style,
        message_filter_func filter
    ) {
        //TODO: Use filter function

        // Table for rapid lookup of tags
        aul::Array_map<std::string_view, fmt::text_style> style_table{};
        style_table.reserve(4);

        // TODO: Optimize population of style_table
        style_table.insert("pl", print_style.plain);
        style_table.insert("ih", print_style.info_header);
        style_table.insert("wh", print_style.warning_header);
        style_table.insert("eh", print_style.error_header);

        const char* print_start = message_buffer.contents().data();
        const char* head = message_buffer.contents().data();
        const char* end = message_buffer.contents().data() + message_buffer.contents().size();

        fmt::text_style current_style = print_style.plain;

        bool inside_of_tag = false;
        for (;head != end; ++head) {
            if (*head != unit_separator) {
                continue;
            }

            std::string_view view{print_start, head};

            if (inside_of_tag) {
                // Handle escape sequence
                if (print_start + 1 == head) {
                    fmt::print(current_style, "{}", unit_separator);
                }

                current_style = style_table.get_or_default(
                    view,
                    print_style.plain
                );
            } else {
                fmt::print(current_style, "{}", view);
            }

            print_start = head + 1;
            inside_of_tag = !inside_of_tag;
        }

        if (!inside_of_tag) {
            std::string_view view{print_start, head};
            fmt::print(current_style, "{}", view);
        }
    }

    std::string to_string(
        const Message_buffer& message_buffer,
        message_filter_func filter
    ) {
        //TODO: Use filter function

        const char* print_start = message_buffer.contents().data();
        const char* head = message_buffer.contents().data();
        const char* end = message_buffer.contents().data() + message_buffer.contents().size();

        std::string ret;
        // A perhaps overly-eager reservation strategy
        ret.reserve(message_buffer.contents().size());

        bool inside_of_tag = false;
        for (;head != end; ++head) {
            if (*head != unit_separator) {
                continue;
            }

            std::string_view view{print_start, head};

            if (!inside_of_tag) {
                ret.append(view);
            }

            print_start = head + 1;
            inside_of_tag = !inside_of_tag;
        }

        if (!inside_of_tag) {
            std::string_view view{print_start, head};
            ret.append(view);
        }

        return ret;
    }

}
