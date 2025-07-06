#include "CacheMapTest.hpp"
#include <gtest/gtest.h>
#include <drogon/CacheMap.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <thread>
#include <chrono>

using namespace drogon;
using namespace std::chrono_literals;

class CacheMapTest : public ::testing::Test
{
protected:
    trantor::EventLoopThread loopThread;
    CacheMap<std::string, std::string> *cache;

    void SetUp() override
    {
        loopThread.run();
        cache = new CacheMap<std::string, std::string>(loopThread.getLoop(),
                                                      0.1,
                                                      4,
                                                      30);
    }

    void TearDown() override
    {
        delete cache;
    }
};

TEST_F(CacheMapTest, InsertAndFind)
{
    for (size_t i = 1; i < 40; i++)
        cache->insert(std::to_string(i), "a", i);
    cache->insert("bla", "");
    cache->insert("zzz", "-");
    std::this_thread::sleep_for(3s);

    EXPECT_FALSE(cache->find("0"));  // doesn't exist
    EXPECT_FALSE(cache->find("1"));  // timeout
    EXPECT_TRUE(cache->find("15"));
    EXPECT_TRUE(cache->find("bla"));
}

TEST_F(CacheMapTest, Erase)
{
    cache->insert("30", "value");
    cache->erase("30");
    EXPECT_FALSE(cache->find("30"));
}

TEST_F(CacheMapTest, ModifyAndAccessOperator)
{
    cache->insert("bla", "");
    cache->modify("bla", [](std::string& s) { s = "asd"; });
    EXPECT_EQ((*cache)["bla"], "asd");
}

TEST_F(CacheMapTest, FindAndFetch)
{
    cache->insert("zzz", "-");
    std::string content;
    cache->findAndFetch("zzz", content);
    EXPECT_EQ(content, "-");
}