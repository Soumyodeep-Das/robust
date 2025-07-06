#include "gtest/gtest.h"
#include "Sha1.hpp"
#include <cstring>

class Sha1Test : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(Sha1Test, EmptyInput)
{
    unsigned char output[20] = {0};
    unsigned char expected[20] = {
        0xda, 0x39, 0xa3, 0xee,
        0x5e, 0x6b, 0x4b, 0x0d,
        0x32, 0x55, 0xbf, 0xef,
        0x95, 0x60, 0x18, 0x90,
        0xaf, 0xd8, 0x07, 0x09
    };
    SHA1(nullptr, 0, output);
    EXPECT_EQ(0, std::memcmp(output, expected, 20));
}

TEST_F(Sha1Test, KnownString)
{
    const char *input = "abc";
    unsigned char output[20] = {0};
    unsigned char expected[20] = {
        0xa9, 0x99, 0x3e, 0x36,
        0x47, 0x06, 0x81, 0x6a,
        0xba, 0x3e, 0x25, 0x71,
        0x78, 0x50, 0xc2, 0x6c,
        0x9c, 0xd0, 0xd8, 0x9d
    };
    SHA1(reinterpret_cast<const unsigned char*>(input), std::strlen(input), output);
    EXPECT_EQ(0, std::memcmp(output, expected, 20));
}

TEST_F(Sha1Test, LongerInput)
{
    const char *input = "The quick brown fox jumps over the lazy dog";
    unsigned char output[20] = {0};
    unsigned char expected[20] = {
        0x2f, 0xd4, 0xe1, 0xc6,
        0x7a, 0x2d, 0x28, 0xfc,
        0xed, 0x84, 0x9e, 0xe1,
        0xbb, 0x76, 0xe7, 0x39,
        0x1b, 0x93, 0xeb, 0x12
    };
    SHA1(reinterpret_cast<const unsigned char*>(input), std::strlen(input), output);
    EXPECT_EQ(0, std::memcmp(output, expected, 20));
}

TEST_F(Sha1Test, OutputBufferNotNull)
{
    const char *input = "test";
    unsigned char output[20] = {0};
    unsigned char *ret = SHA1(reinterpret_cast<const unsigned char*>(input), std::strlen(input), output);
    EXPECT_EQ(ret, output);
}