#include "RangeTestController.hpp"
#include <gtest/gtest.h>
#include <functional>

// Mock classes to enable compilation and testing
class HttpRequestPtr
{
    // Minimal stub for testing
};

class HttpResponsePtr
{
    // Minimal stub for testing
};

class HttpResponse
{
public:
    static HttpResponsePtr newFileResponse(const char* filename)
    {
        (void)filename;
        return HttpResponsePtr{};
    }
    static HttpResponsePtr newFileResponse(const char* filename, size_t offset, size_t length)
    {
        (void)filename;
        (void)offset;
        (void)length;
        return HttpResponsePtr{};
    }
};

class RangeTestControllerTest : public ::testing::Test
{
protected:
    RangeTestController controller;
    HttpRequestPtr req;

    bool callbackCalled;
    HttpResponsePtr receivedResponse;

    void SetUp() override
    {
        callbackCalled = false;
    }

    void callbackFunction(const HttpResponsePtr &resp)
    {
        callbackCalled = true;
        receivedResponse = resp;
    }
};

TEST_F(RangeTestControllerTest, ConstructorCreatesFile)
{
    // The constructor creates the file "./range-test.txt"
    // We can test that the file exists by trying to open it
    RangeTestController localController;
    std::ifstream infile("./range-test.txt", std::ios::binary);
    EXPECT_TRUE(infile.is_open());
    infile.close();
}

TEST_F(RangeTestControllerTest, GetFileCallsCallback)
{
    controller.getFile(req, [this](const HttpResponsePtr &resp) {
        callbackFunction(resp);
    });
    EXPECT_TRUE(callbackCalled);
}

TEST_F(RangeTestControllerTest, GetFileByRangeCallsCallback)
{
    size_t offset = 10;
    size_t length = 20;
    controller.getFileByRange(req, [this](const HttpResponsePtr &resp) {
        callbackFunction(resp);
    }, offset, length);
    EXPECT_TRUE(callbackCalled);
}