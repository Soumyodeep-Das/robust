#include "gtest/gtest.h"
#include "test_controllers.hpp"

TEST(RemoteAPITest, BasicAssertions)
{
    auto client = drogon::HttpClient::newHttpClient("http://localhost:3000");
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/jobs");

    std::promise<void> promise;
    auto future = promise.get_future();

    client->sendRequest(req, [&](drogon::ReqResult res, const drogon::HttpResponsePtr& resp) {
        EXPECT_EQ(res, drogon::ReqResult::Ok);
        EXPECT_NE(resp, nullptr);
        if (resp)
        {
            EXPECT_EQ(resp->getStatusCode(), drogon::k200OK);
            EXPECT_EQ(resp->contentType(), drogon::CT_APPLICATION_JSON);
        }
        promise.set_value();
    });

    // Wait for the asynchronous callback to complete
    future.wait();
}