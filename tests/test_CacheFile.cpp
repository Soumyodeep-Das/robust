#include "CacheFile.hpp"
#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

using namespace drogon;

class CacheFileTest : public ::testing::Test
{
protected:
    std::string testFileName = "test_cachefile.tmp";

    void SetUp() override
    {
        // Remove file if exists
        std::remove(testFileName.c_str());
    }

    void TearDown() override
    {
        std::remove(testFileName.c_str());
    }
};

TEST_F(CacheFileTest, AppendAndLength)
{
    CacheFile cache(testFileName, true);
    const char *text = "Hello, CacheFile!";
    cache.append(text, strlen(text));
    size_t len = cache.length();
    EXPECT_EQ(len, strlen(text));
}

TEST_F(CacheFileTest, DataMapping)
{
    CacheFile cache(testFileName, true);
    const char *text = "Data for mmap test";
    cache.append(text, strlen(text));
    // Flush to file to ensure data is written
    fflush(nullptr);

    char *mappedData = cache.data();
    ASSERT_NE(mappedData, nullptr);
    EXPECT_EQ(std::memcmp(mappedData, text, strlen(text)), 0);
}

TEST_F(CacheFileTest, DataReturnsNullIfNoFile)
{
    CacheFile cache("non_existent_file.tmp", false);
    char *mappedData = cache.data();
    EXPECT_EQ(mappedData, nullptr);
}

TEST_F(CacheFileTest, DestructorDeletesFileIfAutoDelete)
{
    {
        CacheFile cache(testFileName, true);
        const char *text = "Temporary file";
        cache.append(text, strlen(text));
    }
    // After destruction, file should be deleted
    std::ifstream file(testFileName);
    EXPECT_FALSE(file.good());
}

TEST_F(CacheFileTest, DestructorKeepsFileIfNotAutoDelete)
{
    {
        CacheFile cache(testFileName, false);
        const char *text = "Persistent file";
        cache.append(text, strlen(text));
    }
    // After destruction, file should still exist
    std::ifstream file(testFileName);
    EXPECT_TRUE(file.good());
    file.close();
    std::remove(testFileName.c_str());
}