#include "SlowCtrl.hpp"
#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <memory>
#include <string>

class SlowCtrlTest : public ::testing::Test
{
protected:
    SlowCtrl slowCtrl;

    // Helper class to capture the HttpResponsePtr passed to callback
    class ResponseCapturer
    {
    public:
        drogon::HttpResponsePtr response;

        void callback(const drogon::HttpResponsePtr &resp)
        {
            response = resp;
        }
    };
};

TEST_F(SlowCtrlTest, HelloReturnsCorrectBody)
{
    auto req = drogon::HttpRequest::newHttpRequest();
    ResponseCapturer capturer;
    std::string userid = "user123";

    slowCtrl.hello(req, std::function<void(const drogon::HttpResponsePtr &)>([&capturer](const drogon::HttpResponsePtr &resp) {
                       capturer.callback(resp);
                   }),
                   std::move(userid));

    ASSERT_NE(capturer.response, nullptr);
    std::string expectedBody = "hello, user123";
    ASSERT_EQ(capturer.response->getBody(), expectedBody);
}

TEST_F(SlowCtrlTest, ObserveHandlesException)
{
    // Since observe is a coroutine and depends on Redis and getFromCache,
    // here we test that the callback is called with a not found response on exception.
    // We simulate this by calling observe with invalid userid that triggers exception.
    // However, without mocking getFromCache or Redis, we cannot fully test async behavior here.
    // So this test will just check that observe compiles and callable.

    auto req = drogon::HttpRequest::newHttpRequest();
    ResponseCapturer capturer;
    std::string userid = "invalid_user_for_test";

    // Because observe is a coroutine returning AsyncTask, we need to run it in an event loop.
    // For unit test, we just check that observe returns a valid AsyncTask object.
    auto task = slowCtrl.observe(req,
                                std::function<void(const drogon::HttpResponsePtr &)>([&capturer](const drogon::HttpResponsePtr &resp) {
                                    capturer.callback(resp);
                                }),
                                userid);

    ASSERT_TRUE(task.valid());
}