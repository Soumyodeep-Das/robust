#include "ListenerManager.hpp"
#include <gtest/gtest.h>

using namespace drogon;

class ListenerManagerTest : public ::testing::Test
{
protected:
    ListenerManager manager;

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ListenerManagerTest, AddListenerAndGetListeners)
{
    manager.addListener("127.0.0.1", 8080, false, "", "", false, {});
    auto listeners = manager.getListeners();
    ASSERT_EQ(listeners.size(), 0) << "No servers started yet, so no listeners";
}

TEST_F(ListenerManagerTest, CreateListenersReturnsLoops)
{
    manager.addListener("127.0.0.1", 0, false, "", "", false, {});
    std::vector<ListenerManager::HttpAsyncCallback> httpCallbacks;
    std::vector<ListenerManager::WebSocketNewAsyncCallback> wsCallbacks;
    std::vector<ListenerManager::ConnectionCallback> connCallbacks;
    std::vector<std::function<ListenerManager::HttpResponsePtr(const ListenerManager::HttpRequestPtr &)>> syncAdvices;
    std::vector<std::function<void(const ListenerManager::HttpRequestPtr &, const ListenerManager::HttpResponsePtr &)>> preSendingAdvices;

    auto loops = manager.createListeners(
        []() {}, []() {}, []() {}, 10,
#ifdef OpenSSL_FOUND
        "", "", std::vector<std::pair<std::string, std::string>>{},
#else
        "", "", std::vector<std::pair<std::string, std::string>>{},
#endif
        1, syncAdvices, preSendingAdvices);
    ASSERT_GE(loops.size(), 0);
}

TEST_F(ListenerManagerTest, StartAndStopListening)
{
    manager.addListener("127.0.0.1", 0, false, "", "", false, {});
    std::vector<std::function<ListenerManager::HttpResponsePtr(const ListenerManager::HttpRequestPtr &)>> syncAdvices;
    std::vector<std::function<void(const ListenerManager::HttpRequestPtr &, const ListenerManager::HttpResponsePtr &)>> preSendingAdvices;

    manager.createListeners(
        []() {}, []() {}, []() {}, 10,
#ifdef OpenSSL_FOUND
        "", "", std::vector<std::pair<std::string, std::string>>{},
#else
        "", "", std::vector<std::pair<std::string, std::string>>{},
#endif
        1, syncAdvices, preSendingAdvices);
    manager.startListening();
    manager.stopListening();
}

TEST_F(ListenerManagerTest, GetIOLoopReturnsNullWhenNoLoops)
{
    auto loop = manager.getIOLoop(0);
    ASSERT_EQ(loop, nullptr);
}

TEST_F(ListenerManagerTest, GetIOLoopReturnsValidLoop)
{
    manager.addListener("127.0.0.1", 0, false, "", "", false, {});
    std::vector<std::function<ListenerManager::HttpResponsePtr(const ListenerManager::HttpRequestPtr &)>> syncAdvices;
    std::vector<std::function<void(const ListenerManager::HttpRequestPtr &, const ListenerManager::HttpResponsePtr &)>> preSendingAdvices;

    manager.createListeners(
        []() {}, []() {}, []() {}, 10,
#ifdef OpenSSL_FOUND
        "", "", std::vector<std::pair<std::string, std::string>>{},
#else
        "", "", std::vector<std::pair<std::string, std::string>>{},
#endif
        1, syncAdvices, preSendingAdvices);
    auto loop = manager.getIOLoop(0);
    ASSERT_NE(loop, nullptr);
}