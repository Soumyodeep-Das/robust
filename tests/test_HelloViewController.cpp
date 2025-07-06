#include "gtest/gtest.h"
#include "HelloViewController.hpp"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <memory>
#include <string>

using namespace drogon;

class HelloViewControllerTest : public ::testing::Test
{
protected:
    HelloViewController controller;

    // Helper callback to capture the response
    struct CallbackHelper
    {
        HttpResponsePtr response;
        void operator()(const HttpResponsePtr &resp)
        {
            response = resp;
        }
    };
};

TEST_F(HelloViewControllerTest, AsyncHandleHttpRequest_SetsNameParameter)
{
    // Create a HttpRequestPtr with parameter "name" set
    auto req = HttpRequest::newHttpRequest();
    req->setParameter("name", "TestName");

    CallbackHelper cbHelper;

    controller.asyncHandleHttpRequest(req, std::move(std::function<void(const HttpResponsePtr &)>(cbHelper)));

    ASSERT_NE(cbHelper.response, nullptr);
    // The response should be a view response with view name "HelloView"
    EXPECT_EQ(cbHelper.response->getViewName(), "HelloView");

    // The view data should contain the "name" parameter with value "TestName"
    auto viewData = cbHelper.response->getViewData();
    auto it = viewData.find("name");
    ASSERT_NE(it, viewData.end());
    EXPECT_EQ(it->second, "TestName");
}

TEST_F(HelloViewControllerTest, AsyncHandleHttpRequest_EmptyNameParameter)
{
    auto req = HttpRequest::newHttpRequest();
    // No "name" parameter set

    CallbackHelper cbHelper;

    controller.asyncHandleHttpRequest(req, std::move(std::function<void(const HttpResponsePtr &)>(cbHelper)));

    ASSERT_NE(cbHelper.response, nullptr);
    EXPECT_EQ(cbHelper.response->getViewName(), "HelloView");

    auto viewData = cbHelper.response->getViewData();
    auto it = viewData.find("name");
    // If no parameter set, the value should be empty string
    ASSERT_NE(it, viewData.end());
    EXPECT_EQ(it->second, "");
}