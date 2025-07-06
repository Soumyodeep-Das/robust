#include "gtest/gtest.h"
#include "RedisResultSkipped.hpp"
#include <csignal>
#include <cstdlib>

namespace drogon
{
namespace nosql
{
class RedisResultTest : public ::testing::Test
{
protected:
    RedisResult redisResult;
};

TEST_F(RedisResultTest, GetStringForDisplaying_Aborts)
{
    EXPECT_DEATH(redisResult.getStringForDisplaying(), ".*");
}

TEST_F(RedisResultTest, GetStringForDisplayingWithIndent_Aborts)
{
    EXPECT_DEATH(redisResult.getStringForDisplayingWithIndent(0), ".*");
}

TEST_F(RedisResultTest, AsString_Aborts)
{
    EXPECT_DEATH(redisResult.asString(), ".*");
}

TEST_F(RedisResultTest, Type_Aborts)
{
    EXPECT_DEATH(redisResult.type(), ".*");
}

TEST_F(RedisResultTest, AsArray_Aborts)
{
    EXPECT_DEATH(redisResult.asArray(), ".*");
}

TEST_F(RedisResultTest, AsInteger_Aborts)
{
    EXPECT_DEATH(redisResult.asInteger(), ".*");
}

TEST_F(RedisResultTest, IsNil_Aborts)
{
    EXPECT_DEATH(redisResult.isNil(), ".*");
}
}  // namespace nosql
}  // namespace drogon