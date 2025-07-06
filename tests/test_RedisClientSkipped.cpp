#include <gtest/gtest.h>
#include "RedisClientSkipped.hpp"
#include "trantor/net/InetAddress.h"

namespace drogon
{
namespace nosql
{
class RedisClientSkippedTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(RedisClientSkippedTest, NewRedisClientAborts)
{
    trantor::InetAddress addr("127.0.0.1", 6379);
    // Since newRedisClient calls abort(), we cannot call it directly in a normal test.
    // Instead, we can test that the function exists and is callable by checking the signature.
    // Proper testing of abort() would require death tests.
    // Using GoogleTest death test to verify abort behavior:
    EXPECT_DEATH({
        RedisClient::newRedisClient(addr, 1, "", 0);
    }, "Redis is not supported by drogon");
}
}  // namespace nosql
}  // namespace drogon