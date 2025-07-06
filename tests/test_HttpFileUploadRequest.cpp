#include "HttpFileUploadRequest.hpp"
#include <gtest/gtest.h>
#include <drogon/UploadFile.h>

using namespace drogon;

class HttpFileUploadRequestTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HttpFileUploadRequestTest, ConstructorSetsBoundaryAndFiles)
{
    std::vector<UploadFile> files;
    UploadFile file;
    file.filename = "test.txt";
    file.contentType = "text/plain";
    file.filePath = "/tmp/test.txt";
    files.push_back(file);

    HttpFileUploadRequest req(files);

    // The files_ member is private; we cannot access it directly.
    // We test indirectly by checking content type contains boundary string.
    std::string contentType = req.getHeader("Content-Type");
    ASSERT_NE(contentType.find("multipart/form-data; boundary="), std::string::npos);

    // Check method and version
    EXPECT_EQ(req.getMethod(), drogon::Post);
    EXPECT_EQ(req.getVersion(), drogon::Version::kHttp11);
}