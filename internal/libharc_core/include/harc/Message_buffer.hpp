#ifndef HARC_MESSAGE_BUFFER_HPP
#define HARC_MESSAGE_BUFFER_HPP

#include <harc/Util.hpp>

#include <fmt/color.h>

#include <aul/Span.hpp>

#include <vector>
#include <string>
#include <cstddef>
#include <span>
#include <array>

namespace harc {

    //=====================================================
    // Message markup
    //=====================================================

    ///
    /// Enum class representing the different message types supported by the
    /// Message_buffer class.
    ///
    enum class Message_type : std::uint8_t {
        INFO,
        ADVICE,
        WARNING,
        ERROR
    };

    //=====================================================
    // Message filtering methods
    //=====================================================

    ///
    /// Type of functions which are used as filters in the context of language.
    ///
    using message_filter_func = bool (*)(Message_type message_type, std::string_view category);

    template<bool Info, bool Advice, bool Warning, bool Error>
    bool filter_messages(Message_type message_type, std::string_view category) {
        bool t0 = Info    && (Message_type::INFO    == message_type);
        bool t1 = Advice  && (Message_type::ADVICE  == message_type);
        bool t2 = Warning && (Message_type::WARNING == message_type);
        bool t3 = Error   && (Message_type::ERROR   == message_type);

        return t0 | t1 | t2 | t3;
    }

    ///
    /// Message filter which passes all inputs.
    ///
    /// \param message_type
    /// \param category
    /// \return
    inline bool message_filter_none(Message_type message_type, std::string_view category) {
        return true;
    }

    ///
    ///  Message filter function which filters info messages.
    ///
    /// \param message_type
    /// \param category
    /// \return
    inline bool message_filter_low(Message_type message_type, std::string_view category) {
        return filter_messages<false, true, true, true>(message_type, category);
    }

    ///
    /// Message filter function which filter info and advice messages.
    ///
    /// \param message_type
    /// \param category
    /// \return
    inline bool message_filter_medium(Message_type message_type, std::string_view category) {
        return filter_messages<false, false, true, true>(message_type, category);
    }

    ///
    /// Message filter function which filters info, advice, and warning messages.
    ///
    /// \param message_type
    /// \param category
    /// \return
    inline bool message_filter_high(Message_type message_type, std::string_view category) {
        return filter_messages<false, false, false, true>(message_type, category);
    }

    ///
    /// Message filter function which filters all messages.
    ///
    /// \param message_type
    /// \param category
    /// \return
    inline bool message_filter_all(Message_type message_type, std::string_view category) {
        return false;
    }

    //=====================================================
    // Message markup
    //=====================================================

    ///
    ///
    ///
    ///
    ///
    enum class Message_markup_tag : std::uint8_t {
        NONE,
        PLAIN,
        INFO_HEADER,
        ADVICE_HEADER,
        WARNING_HEADER,
        ERROR_HEADER
    };



    ///
    /// This class is used to color text using Harc's Message Markup format.
    /// It's primarily meant as a helper class to be used by the
    /// harc::Message_buffer class.
    ///
    /// Harc's Message Markup is used so that colored messages may be
    /// transmitted across multiple machines in a portable fashion.
    ///
    class Message_markup_builder {
    public:

        //=================================================
        // Mutators
        //=================================================

        ///
        /// Appends a string to the internal buffer using the specified tag.
        ///
        /// \param str String to print
        /// \param tag Tag to apply to string
        void append(
            std::string_view str,
            Message_markup_tag type = Message_markup_tag::PLAIN
        );

        ///
        ///  Appends multiple strings to the internal buffer using the specified
        ///  tags.
        ///
        /// \param strings_tags Multispan over strings and their markup tags
        void append(
            aul::Multispan<
                const std::string_view,
                const Message_markup_tag
            > strings_tags
        );

        ///
        /// Pre-allocates memory for future use by the message buffer object.
        ///
        /// \param s Amount of memory to reserve for this buffer.
        void reserve(std::size_t s) {
            buffer.reserve(s);
        }

        ///
        /// Removes the current contents of the message buffer.
        ///
        void clear() {
            buffer.clear();
        }

        //=================================================
        // Accessors
        //=================================================

        ///
        /// Get the raw contents of the message buffer.
        ///
        /// \return View over buffer's contents
        [[nodiscard]]
        std::string_view contents() const {
            return buffer;
        }

        ///
        ///
        /// \return True if the buffer's content is empty
        [[nodiscard]]
        bool empty() const {
            return buffer.empty();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Message_markup_tag last_tag = Message_markup_tag::NONE;

        std::string buffer;

    };

    ///
    /// This class represents a buffer that is used to store message that are
    /// produced by the compilation process before being presented to the user.
    ///
    /// TODO:
    /// A message is comprised from three different pieces of data: a message
    /// type (stored as a single byte), a message category (stored as a
    /// null-terminated string), and the message's string itself, annotated via
    /// message markup.
    ///
    class Message_buffer {
    public:

        //TODO: Add iterators to Message_buffer class
        //TODO: Add message categories

        //=================================================
        // -ctors
        //=================================================

        explicit Message_buffer(message_filter_func filter):
            filter(filter) {}

        Message_buffer() = default;
        Message_buffer(const Message_buffer&) = default;
        Message_buffer(Message_buffer&&) noexcept = default;
        ~Message_buffer() = default;

        //=================================================
        // Assignment Operators
        //=================================================

        Message_buffer& operator=(const Message_buffer&) = default;
        Message_buffer& operator=(Message_buffer&&) = default;

        //=================================================
        // Info methods
        //=================================================

        ///
        /// Enqueues an info message
        ///
        /// \param message Message to report
        void info(
            std::string_view message
        );

        ///
        /// Enqueues an info message involving a path
        ///
        /// \param message Message to report
        /// \param path Path to file from which the message is sourced
        void info(
            std::string_view message,
            std::string_view path
        );

        ///
        /// Enqueues an info message involving a path and line, column number
        ///
        /// \param message Message to report
        /// \param path Path to file from which the message is sourced
        /// \param location Line and column number from which error is sourced
        void info(
            std::string_view message,
            std::string_view path,
            std::uint32_t line,
            std::uint32_t column
        );

        //=================================================
        // Advice methods
        //=================================================

        void advice(
            std::string_view message
        );

        void advice(
            std::string_view message,
            std::string_view path
        );

        void advice(
            std::string_view message,
            std::string_view path,
            std::uint32_t line,
            std::uint32_t column
        );

        //=================================================
        // Warning methods
        //=================================================

        void warning(
            std::string_view message
        );

        void warning(
            std::string_view message,
            std::string_view path
        );

        void warning(
            std::string_view message,
            std::string_view path,
            std::uint32_t line,
            std::uint32_t column
        );

        //=================================================
        // Error methods
        //=================================================

        void error(
            std::string_view message
        );

        void error(
            std::string_view message,
            std::string_view path
        );

        void error(
            std::string_view message,
            std::string_view path,
            std::uint32_t line,
            std::uint32_t column
        );

        //=================================================
        // Mutators
        //=================================================

        ///
        ///
        /// \param buffer
        void append(const Message_buffer& buffer);

        ///
        ///
        /// \param buffer
        void append(Message_buffer&& buffer);

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        aul::Span<const char> contents() const {
            return aul::Span<const char>{buffer.data(), buffer.size()};
        }

        void reserve(std::size_t s) {
            buffer.reserve(s);
        }

        void clear() {
            buffer = {};
        }

        [[nodiscard]]
        bool empty() const {
            return buffer.empty();
        }

        [[nodiscard]]
        message_filter_func message_filter() const {
            return filter;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::vector<char> buffer;

        message_filter_func filter = message_filter_medium;

    };

    //=====================================================
    // Print styles
    //=====================================================

    ///
    /// This struct represents the formatting of text that is output by the
    /// harc::print(const Message_buffer&) method. It controls effects such as
    /// bolding, italicization, and color.
    ///
    struct Print_style {
        fmt::text_style plain;

        fmt::text_style info_header;
        fmt::text_style advice_header;
        fmt::text_style warning_header;
        fmt::text_style error_header;
    };



    ///
    /// The default print style used by the print(const Message_buffer&, const Print_style&) function
    ///
    constexpr Print_style default_print_style {
        .plain = fmt::text_style{},
        .info_header = fmt::emphasis::bold | fmt::fg(fmt::color::alice_blue),
        .advice_header = fmt::emphasis::bold | fmt::fg(fmt::color::beige),
        .warning_header = fmt::emphasis::bold | fmt::fg(fmt::color::yellow),
        .error_header = fmt::emphasis::bold | fmt::fg(fmt::color::red)
    };

    ///
    /// A print style that outputs text without any formatting of styling
    ///
    constexpr Print_style plain_print_style {
        .plain = fmt::text_style{},
        .info_header = fmt::text_style{},
        .advice_header = fmt::text_style{},
        .warning_header = fmt::text_style{},
        .error_header = fmt::text_style{}
    };

    //=====================================================
    // Printing methods
    //=====================================================

    ///
    /// Prints the contents of a Message_buffer object to STDOUT.
    ///
    /// \param message_buffer Arbitrary Message_buffer object
    /// \param use_color Indicates whether output should be printed using color
    void print(
        const Message_buffer& message_buffer,
        const Print_style& print_style = default_print_style,
        message_filter_func filter = message_filter_medium
    );

    ///
    /// Extracts the raw text from a Message_buffer object.
    ///
    /// \param message_buffer Arbitrary Message_buffer object
    /// \param filter Filter function
    /// \return String representation of Message_buffer object
    std::string to_string(
        const Message_buffer& message_buffer,
        message_filter_func filter = message_filter_medium
    );

}

#endif //HARC_MESSAGE_BUFFER_HPP
