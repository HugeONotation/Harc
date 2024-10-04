#ifndef HARC_ALGORITHMS_TESTS_HPP
#define HARC_ALGORITHMS_TESTS_HPP

#include <harc/common/Algorithms.hpp>

namespace harc::tests {

    TEST(demerau_levenstein_edit_distance_ascii, empty_strings) {
        const std::string_view str0 = "";
        const std::string_view str1 = "";
        std::size_t diff  = damerau_levenstein_edit_distance_ascii(str0, str1);
        EXPECT_EQ(0, diff);
    }

    TEST(demerau_levenstein_edit_distance_ascii, one_empty_string) {
        std::size_t diff0  = damerau_levenstein_edit_distance_ascii(
            "a",
            ""
        );
        EXPECT_EQ(1, diff0);


        std::size_t diff1  = damerau_levenstein_edit_distance_ascii(
            "ab",
            ""
        );
        EXPECT_EQ(2, diff1);


        std::size_t diff2  = damerau_levenstein_edit_distance_ascii(
            "abc",
            ""
        );
        EXPECT_EQ(3, diff2);
    }

    TEST(demerau_levenstein_edit_distance_ascii, swapped_characters) {
        std::size_t diff0  = damerau_levenstein_edit_distance_ascii(
            "ab",
            "ba"
        );
        EXPECT_EQ(1, diff0);

        std::size_t diff1  = damerau_levenstein_edit_distance_ascii(
            "aba",
            "baa"
        );
        EXPECT_EQ(1, diff1);

        std::size_t diff2  = damerau_levenstein_edit_distance_ascii(
            "abaa",
            "baaa"
        );
        EXPECT_EQ(1, diff2);

        std::size_t diff3  = damerau_levenstein_edit_distance_ascii(
            "aab",
            "aba"
        );
        EXPECT_EQ(1, diff3);

        std::size_t diff4  = damerau_levenstein_edit_distance_ascii(
            "aaab",
            "aaba"
        );
        EXPECT_EQ(1, diff4);
    }

    TEST(most_similar, empty_list) {

    }

}

#endif //HARC_ALGORITHMS_TESTS_HPP
