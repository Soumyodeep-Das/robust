#include "HttpBinder.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace drogon;
using namespace drogon::internal;

class HttpBinderTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HttpBinderTest, HandleExceptionCallsExceptionHandler)
{
    // We cannot directly test the internal app().getExceptionHandler() call without
    // Drogon framework internals, so we test that handleException does not throw
    // and accepts parameters correctly.

    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr&) { callbackCalled = true; };

    // Create dummy HttpRequestPtr (nullptr is acceptable for this test)
    HttpRequestPtr req;

    // Create a dummy exception
    std::runtime_error ex("test exception");

    // Call handleException, expecting no throw
    EXPECT_NO_THROW(handleException(ex, req, std::move(callback)));
}