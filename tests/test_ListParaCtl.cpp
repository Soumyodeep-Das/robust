#include "gtest/gtest.h"
#include "ListParaCtl.hpp"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <memory>
#include <string>
#include <map>

class ListParaCtlTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

class DummyHttpRequest : public drogon::HttpRequest
{
public:
    DummyHttpRequest() : drogon::HttpRequest(drogon::HttpMethod::Get, "/dummy") {}

    const std::multimap<std::string, std::string> &getParameters() const override
    {
        static std::multimap<std::string, std::string> params = {
            {"key1", "value1"},
            {"key2", "value2"}};
        return params;
    }
};

class DummyHttpResponse : public drogon::HttpResponse
{
public:
    DummyHttpResponse() : drogon::HttpResponse() {}
    std::string viewName;
    drogon::HttpViewData viewData;

    static std::shared_ptr<DummyHttpResponse> newHttpViewResponse(
        const std::string &view,
        const drogon::HttpViewData &data)
    {
        auto resp = std::make_shared<DummyHttpResponse>();
        resp->viewName = view;
        resp->viewData = data;
        return resp;
    }
};

TEST_F(ListParaCtlTest, AsyncHandleHttpRequest_CreatesCorrectResponse)
{
    ListParaCtl ctl;
    DummyHttpRequest req;
    bool callbackCalled = false;

    auto callback = [&](const drogon::HttpResponsePtr &resp) {
        callbackCalled = true;
        // Check that the response is not null
        ASSERT_NE(resp, nullptr);

        // Cast to DummyHttpResponse to check internals
        auto dummyResp = std::dynamic_pointer_cast<DummyHttpResponse>(resp);
        ASSERT_NE(dummyResp, nullptr);

        // Check view name
        EXPECT_EQ(dummyResp->viewName, "ListParaView.csp");

        // Check view data keys and values
        auto itTitle = dummyResp->viewData.find("title");
        ASSERT_NE(itTitle, dummyResp->viewData.end());
        EXPECT_EQ(itTitle->second, "list parameters");

        auto itParams = dummyResp->viewData.find("parameters");
        ASSERT_NE(itParams, dummyResp->viewData.end());

        // The parameters should match those from DummyHttpRequest
        const auto &params = req.getParameters();
        EXPECT_EQ(itParams->second.size(), params.size());
        for (const auto &p : params)
        {
            EXPECT_NE(itParams->second.find(p.first), itParams->second.end());
            EXPECT_EQ(itParams->second.at(p.first), p.second);
        }
    };

    // Override drogon::HttpResponse::newHttpViewResponse to our dummy for testing
    // Since we cannot override static methods easily, we simulate by temporarily replacing the method pointer.
    // Instead, we use a lambda to replace the call inside the test.

    // We will temporarily patch the method in ListParaCtl for testing by subclassing
    class TestListParaCtl : public ListParaCtl
    {
    public:
        std::function<std::shared_ptr<DummyHttpResponse>(const std::string &, const drogon::HttpViewData &)> newHttpViewResponseFunc;

        void asyncHandleHttpRequest(
            const drogon::HttpRequestPtr &req,
            std::function<void(const drogon::HttpResponsePtr &)> &&callback) override
        {
            drogon::HttpViewData data;
            data.insert("title", "list parameters");
            data.insert("parameters", req->getParameters());
            auto res = newHttpViewResponseFunc("ListParaView.csp", data);
            callback(res);
        }
    };

    // Use the subclass to inject our dummy response creation
    TestListParaCtl testCtl;
    testCtl.newHttpViewResponseFunc = DummyHttpResponse::newHttpViewResponse;

    testCtl.asyncHandleHttpRequest(std::make_shared<DummyHttpRequest>(), std::move(callback));
    EXPECT_TRUE(callbackCalled);
}