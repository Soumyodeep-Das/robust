#include "gtest/gtest.h"
#include "MultiPart.hpp"
#include <memory>

using namespace drogon;

class MultiPartParserTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MultiPartParserTest, GetFilesInitiallyEmpty)
{
    MultiPartParser parser;
    const auto &files = parser.getFiles();
    EXPECT_TRUE(files.empty());
}

TEST_F(MultiPartParserTest, GetParametersInitiallyEmpty)
{
    MultiPartParser parser;
    const auto &params = parser.getParameters();
    EXPECT_TRUE(params.empty());
}

TEST_F(MultiPartParserTest, GetFilesMapEmptyWhenNoFiles)
{
    MultiPartParser parser;
    auto filesMap = parser.getFilesMap();
    EXPECT_TRUE(filesMap.empty());
}

// Since parse requires a valid HttpRequestPtr with specific content,
// and HttpRequestImpl is internal, we cannot easily test parse without
// mocking or complex setup. We test parseEntity with invalid data.

TEST_F(MultiPartParserTest, ParseEntityInvalidDataReturnsMinusOne)
{
    MultiPartParser parser;
    const char *begin = "invalid data";
    const char *end = begin + strlen(begin);
    int ret = parser.parseEntity(begin, end);
    EXPECT_EQ(ret, -1);
}

TEST_F(MultiPartParserTest, ParseEntityParameterOnly)
{
    MultiPartParser parser;
    // Compose a minimal entity with a parameter only (no filename)
    const char *entity =
        "Content-Disposition: form-data; name=\"test\"\r\n\r\nvalue";
    int ret = parser.parseEntity(entity, entity + strlen(entity));
    EXPECT_EQ(ret, 0);
    const auto &params = parser.getParameters();
    auto it = params.find("test");
    EXPECT_NE(it, params.end());
    EXPECT_EQ(it->second, "value");
}

TEST_F(MultiPartParserTest, ParseEntityFile)
{
    MultiPartParser parser;
    // Compose a minimal entity with a file
    const char *entity =
        "Content-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\n\r\nfilecontent";
    int ret = parser.parseEntity(entity, entity + strlen(entity));
    EXPECT_EQ(ret, 0);
    const auto &files = parser.getFiles();
    EXPECT_EQ(files.size(), 1u);
    EXPECT_EQ(files[0].getItemName(), "file1");
    EXPECT_EQ(files[0].getFileName(), "test.txt");
}