#ifndef HARMONIA_NETWORKING_HPP
#define HARMONIA_NETWORKING_HPP

#include <aul/Span.hpp>
#include <bit>

namespace harc {

    ///
    /// Enum used to represent type of data carried by network message
    ///
    enum class Message_type : std::uint8_t {
        NULL_MESSAGE_TYPE,
        CLIENT_REGISTRATION,
        CLIENT_UNREGISTRATION,
        CLIENT_WORK_REQUEST,
        SOURCE_TRANSFER,



        // Do not use more than 7 bits!
    };

    ///
    /// Enum used to represent endianness of messages.
    ///
    /// Used as an alternative to std::endian because its values are
    /// implementation defined and hence not guaranteed to fit within just a
    /// single bit field when used in the Message_header struct.
    ///
    enum class Endianness : std::uint8_t {
        LITTLE = 0,
        BIG = 1,
        NATIVE = int(std::endian::native == std::endian::big),
    };

    ///
    /// A struct representing a block of data too be sent to another instance of
    /// Harc via network connection.
    ///
    struct Message_header {

        ///
        /// Indicates type of message being sent.
        ///
        Message_type type : 7 = Message_type::NULL_MESSAGE_TYPE;

        ///
        /// Bit field indicating endianness of message
        ///
        Endianness endianness : 1 = Endianness::NATIVE;

        ///
        /// Miscellaneous id field used for identifying contents of message
        ///
        std::uint32_t id : 24 = 0;

        ///
        /// Indicates number of bytes in message
        ///
        std::uint32_t size = 0;

    };

    static_assert(sizeof(Message_header) == 8);

}

#endif //HARMONIA_NETWORKING_HPP
