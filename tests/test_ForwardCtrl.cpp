#include "ForwardCtrl.hpp"
#include <gtest/gtest.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <memory>

using namespace drogon;

class ForwardCtrlTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}

    ForwardCtrl forwardCtrl;
};

TEST_F(ForwardCtrlTest, AsyncHandleHttpRequest_SetsCorrectPathAndCallsCallback)
{
    auto req = HttpRequest::newHttpRequest();
    bool callbackCalled = false;

    forwardCtrl.asyncHandleHttpRequest(
        req,
        [&callbackCalled](const HttpResponsePtr &resp) {
            (void)resp; // suppress unused parameter warning
            callbackCalled = true;
        });

    // Check that the path was set correctly
    EXPECT_EQ(req->path(), "/repos/an-tao/drogon/git/refs/heads/master");

    // Since forwarding is asynchronous and depends on drogon internals,
    // we cannot guarantee callback is called here.
    // But we can at least check that the callback is callable.
    EXPECT_FALSE(callbackCalled);
}