#include <gtest/gtest.h>
#include "Base64Test.hpp"

TEST(Base64Test, EncodeDecode)
{
    std::string in{"drogon framework"};
    auto encoded = drogon::utils::base64Encode(reinterpret_cast<const unsigned char *>(in.data()), in.length());
    auto decoded = drogon::utils::base64Decode(encoded);
    EXPECT_EQ(encoded, "ZHJvZ29uIGZyYW1ld29yaw==");
    EXPECT_EQ(decoded, in);
}

TEST(Base64Test, LongString)
{
    std::string in;
    in.reserve(100000);
    for (int i = 0; i < 100000; ++i)
    {
        in.append(1, static_cast<char>(i));
    }
    auto encoded = drogon::utils::base64Encode(reinterpret_cast<const unsigned char *>(in.data()), in.length());
    auto decoded = drogon::utils::base64Decode(encoded);
    EXPECT_EQ(decoded, in);
}

TEST(Base64Test, URLSafe)
{
    std::string in{"drogon framework"};
    auto encoded = drogon::utils::base64Encode(reinterpret_cast<const unsigned char *>(in.data()), in.length(), true);
    auto decoded = drogon::utils::base64Decode(encoded);
    EXPECT_EQ(encoded, "ZHJvZ29uIGZyYW1ld29yaw==");
    EXPECT_EQ(decoded, in);
}

TEST(Base64Test, LongURLSafe)
{
    std::string in;
    in.reserve(100000);
    for (int i = 0; i < 100000; ++i)
    {
        in.append(1, static_cast<char>(i));
    }
    auto encoded = drogon::utils::base64Encode(reinterpret_cast<const unsigned char *>(in.data()), in.length(), true);
    auto decoded = drogon::utils::base64Decode(encoded);
    EXPECT_EQ(decoded, in);
}