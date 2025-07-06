#include "gtest/gtest.h"
#include "LCONV_SIZE.hpp"

TEST(LCONV_SIZE, InfoSizeStartsWithINFO) {
    ASSERT_GE(sizeof(info_size), 5);
    EXPECT_EQ(info_size[0], 'I');
    EXPECT_EQ(info_size[1], 'N');
    EXPECT_EQ(info_size[2], 'F');
    EXPECT_EQ(info_size[3], 'O');
    EXPECT_EQ(info_size[4], ':');
}

TEST(LCONV_SIZE, InfoSizeFormat) {
    // Check that the size digits are numeric characters
    for (int i = 10; i <= 14; ++i) {
        if (info_size[i] == '\0') break;
        EXPECT_GE(info_size[i], '0');
        EXPECT_LE(info_size[i], '9');
    }
}

TEST(LCONV_SIZE, InfoSizeNullTerminated) {
    // Ensure string is null terminated
    size_t len = 0;
    while(info_size[len] != '\0' && len < 100) ++len;
    EXPECT_LT(len, 100);
}

#if defined(KEY)
TEST(LCONV_SIZE, InfoSizeContainsKey) {
    // Check that key is present in the string after size digits
    const char* key_start = nullptr;
    for (size_t i = 0; info_size[i] != '\0'; ++i) {
        if (info_size[i] == 'k' && info_size[i+1] == 'e' && info_size[i+2] == 'y' && info_size[i+3] == '[') {
            key_start = &info_size[i];
            break;
        }
    }
    ASSERT_NE(key_start, nullptr);
    // Check that key ends with ']'
    const char* p = key_start;
    bool found_end = false;
    while (*p != '\0') {
        if (*p == ']') {
            found_end = true;
            break;
        }
        ++p;
    }
    EXPECT_TRUE(found_end);
}
#endif