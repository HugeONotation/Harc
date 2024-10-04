#ifndef HARC_TESTS_MESSAGE_BUFFER_HPP
#define HARC_TESTS_MESSAGE_BUFFER_HPP

#include <harc/Message_buffer.hpp>

namespace harc::tests {

    TEST(Message_buffer_tests, Empty) {
        harc::Message_buffer buffer;

        auto str = harc::to_string(buffer);

        EXPECT_EQ(str, std::string{});
    }

    TEST(Message_buffer_tests, Single_info_message) {
        harc::Message_buffer buffer;
        buffer.info("Dummy message!");

        auto str = harc::to_string(buffer);
        EXPECT_FALSE(str.empty());
    }

    TEST(Message_buffer_tests, Single_info_message_with_path) {
        harc::Message_buffer buffer;
        buffer.info("Dummy message!", "assets/test_files/tokens.hm");

        auto str = harc::to_string(buffer);
        EXPECT_FALSE(str.empty());
    }

    TEST(Message_buffer_tests, Single_warning_message_with_path_and_location) {
        harc::Message_buffer buffer;
        buffer.info("Dummy message!", "assets/test_files/tokens.hm", 10, 10);

        auto str = harc::to_string(buffer);
        EXPECT_FALSE(str.empty());
    }

    TEST(Message_buffer_tests, Single_error_message) {
        harc::Message_buffer buffer;
        buffer.error("Dummy message!");

        auto str = harc::to_string(buffer);
        EXPECT_FALSE(str.empty());
    }

}

#endif //HARC_TESTS_MESSAGE_BUFFER_HPP
