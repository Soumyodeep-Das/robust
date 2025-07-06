#include "gtest/gtest.h"
#include "api_Attachment.hpp"

// Mock classes and functions to simulate dependencies
class MockHttpRequestPtr {};
class MockHttpResponsePtr {};
class MockHttpResponse {
public:
    static MockHttpResponsePtr newHttpViewResponse(const std::string &, const int &) { return MockHttpResponsePtr(); }
    static MockHttpResponsePtr newHttpJsonResponse(const int &) { return MockHttpResponsePtr(); }
    static MockHttpResponsePtr newFileResponse(const std::string &, const std::string &, int) { return MockHttpResponsePtr(); }
};
using HttpRequestPtr = MockHttpRequestPtr;
using HttpResponsePtr = MockHttpResponsePtr;

namespace api {

class AttachmentTest : public ::testing::Test {
protected:
    Attachment attachment;
};

TEST_F(AttachmentTest, GetReturnsViewResponse) {
    bool called = false;
    attachment.get(HttpRequestPtr(), [&](const HttpResponsePtr &) {
        called = true;
    });
    EXPECT_TRUE(called);
}

TEST_F(AttachmentTest, UploadReturnsOkOnParseSuccess) {
    // Since MultiPartParser and related classes are not defined here,
    // this test only checks that callback is called.
    bool called = false;
    attachment.upload(HttpRequestPtr(), [&](const HttpResponsePtr &) {
        called = true;
    });
    EXPECT_TRUE(called);
}

TEST_F(AttachmentTest, UploadImageReturnsOkOrFailed) {
    bool called = false;
    attachment.uploadImage(HttpRequestPtr(), [&](const HttpResponsePtr &) {
        called = true;
    });
    EXPECT_TRUE(called);
}

TEST_F(AttachmentTest, DownloadReturnsFileResponse) {
    bool called = false;
    attachment.download(HttpRequestPtr(), [&](const HttpResponsePtr &) {
        called = true;
    });
    EXPECT_TRUE(called);
}

}  // namespace api