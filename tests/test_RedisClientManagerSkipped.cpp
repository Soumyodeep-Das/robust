#include "gtest/gtest.h"
#include "RedisClientManagerSkipped.hpp"

class RedisClientManagerTest : public ::testing::Test
{
protected:
    drogon::nosql::RedisClientManager manager;
};

TEST_F(RedisClientManagerTest, CreateRedisClientsDoesNothing)
{
    std::vector<trantor::EventLoop *> loops;
    // Should not throw or crash
    EXPECT_NO_THROW(manager.createRedisClients(loops));
}

TEST_F(RedisClientManagerTest, CreateRedisClientAborts)
{
    // The createRedisClient function calls abort(), which terminates the program.
    // GoogleTest does not support testing abort directly.
    // We can test that calling it causes abnormal termination using death test.
    // However, death tests require special setup, so we use ASSERT_DEATH macro.

    ASSERT_DEATH(
        manager.createRedisClient("name", "host", 6379, "password", 1, true, 1.0, 0),
        "Redis is not supported by drogon, please install the hiredis library first.");
}