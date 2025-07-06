#include "BrotliTest.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace drogon::utils;

TEST(BrotliTest, shortText)
{
    std::string source{"123中文顶替要枯械"};
    auto compressed = brotliCompress(source.data(), source.length());
    auto decompressed = brotliDecompress(compressed.data(), compressed.size());
    EXPECT_EQ(source, decompressed);
}

TEST(BrotliTest, longText)
{
    std::string source;
    for (size_t i = 0; i < 100000; i++)
    {
        source.append(std::to_string(i));
    }
    auto compressed = brotliCompress(source.data(), source.length());
    auto decompressed = brotliDecompress(compressed.data(), compressed.size());
    EXPECT_EQ(source, decompressed);
}