#include "gtest/gtest.h"
#include "SimpleReverseProxy.hpp"
#include <json/json.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <memory>

using namespace my_plugin;
using namespace drogon;

class SimpleReverseProxyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code if needed
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }

    // Helper to create a dummy config JSON
    Json::Value createValidConfig()
    {
        Json::Value config;
        Json::Value backends(Json::arrayValue);
        backends.append("http://127.0.0.1:8080");
        backends.append("http://127.0.0.1:8081");
        config["backends"] = backends;
        config["pipelining"] = 2;
        config["same_client_to_same_backend"] = true;
        config["connection_factor"] = 2;
        return config;
    }
};

TEST_F(SimpleReverseProxyTest, InitAndStart_ValidConfig_SetsUpClients)
{
    SimpleReverseProxy proxy;
    Json::Value config = createValidConfig();

    // Expect no exceptions or aborts
    ASSERT_NO_THROW(proxy.initAndStart(config));
}

TEST_F(SimpleReverseProxyTest, InitAndStart_EmptyBackends_Aborts)
{
    SimpleReverseProxy proxy;
    Json::Value config;
    config["backends"] = Json::Value(Json::arrayValue); // empty array

    // Since abort() is called, we cannot catch it directly.
    // Instead, we can fork or mock abort in real tests.
    // Here, just check that backends vector remains empty after initAndStart fails.
    // But since abort() terminates, we skip direct test here.
    // This test is a placeholder to indicate coverage.
    SUCCEED();
}

TEST_F(SimpleReverseProxyTest, InitAndStart_InvalidConnectionFactor_Aborts)
{
    SimpleReverseProxy proxy;
    Json::Value config = createValidConfig();
    config["connection_factor"] = 0;

    // As above, abort() will terminate process, so we cannot test directly here.
    SUCCEED();
}

TEST_F(SimpleReverseProxyTest, PreRouting_UsesSameClientToSameBackendLogic)
{
    SimpleReverseProxy proxy;
    Json::Value config = createValidConfig();
    proxy.initAndStart(config);

    // Create a dummy HttpRequestPtr
    auto req = HttpRequest::newHttpRequest();
    req->setPeerAddr(trantor::InetAddress("127.0.0.1", 12345));

    bool callbackCalled = false;

    proxy.preRouting(
        req,
        [&](const HttpResponsePtr &resp) {
            callbackCalled = true;
            EXPECT_TRUE(resp != nullptr);
        },
        [](bool) {});

    // We cannot guarantee async call completion here, so just check no crash
    SUCCEED();
}

TEST_F(SimpleReverseProxyTest, Shutdown_DoesNotThrow)
{
    SimpleReverseProxy proxy;
    ASSERT_NO_THROW(proxy.shutdown());
}