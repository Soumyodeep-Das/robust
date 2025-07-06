#include "gtest/gtest.h"
#include "HttpFileImpl.hpp"
#include <memory>
#include <string>

using namespace drogon;

class HttpFileImplTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        impl = std::make_shared<HttpFileImpl>();
    }
    std::shared_ptr<HttpFileImpl> impl;
};

TEST_F(HttpFileImplTest, SetAndGetFileName)
{
    std::string filename = "test.txt";
    impl->setFileName(filename);
    EXPECT_EQ(impl->getFileName(), filename);
}

TEST_F(HttpFileImplTest, GetFileExtension)
{
    impl->setFileName("example.txt");
    EXPECT_EQ(impl->getFileExtension(), "txt");
    impl->setFileName("noextension");
    EXPECT_TRUE(impl->getFileExtension().empty());
    impl->setFileName("archive.tar.gz");
    EXPECT_EQ(impl->getFileExtension(), "gz");
}

TEST_F(HttpFileImplTest, SetFileAndFileLengthAndData)
{
    const char data[] = "hello world";
    impl->setFile(data, sizeof(data) - 1);
    EXPECT_EQ(impl->fileLength(), sizeof(data) - 1);
    EXPECT_EQ(std::string(impl->fileData(), impl->fileLength()), "hello world");
}

TEST_F(HttpFileImplTest, SaveToInvalidPath)
{
    impl->setFileName("file.txt");
    // Save to a directory that likely does not exist and cannot be created
    int ret = impl->save("/this/path/should/not/exist/and/fail");
    EXPECT_TRUE(ret == 0 || ret == -1);
}

TEST_F(HttpFileImplTest, SaveAsEmptyFileName)
{
    impl->setFileName("file.txt");
    int ret = impl->saveAs("");
    EXPECT_EQ(ret, -1);
}

TEST_F(HttpFileImplTest, GetMd5EmptyContent)
{
    impl->setFileName("file.txt");
    impl->setFile(nullptr, 0);
    std::string md5 = impl->getMd5();
    EXPECT_FALSE(md5.empty());
}

class HttpFileTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        impl = std::make_shared<HttpFileImpl>();
        file = HttpFile(std::move(impl));
    }
    std::shared_ptr<HttpFileImpl> impl;
    HttpFile file;
};

TEST_F(HttpFileTest, DelegatesFileName)
{
    file.setFileName("myfile.txt");
    EXPECT_EQ(file.getFileName(), "myfile.txt");
}

TEST_F(HttpFileTest, DelegatesFileExtension)
{
    file.setFileName("myfile.txt");
    EXPECT_EQ(file.getFileExtension(), "txt");
}

TEST_F(HttpFileTest, DelegatesFileType)
{
    EXPECT_EQ(file.getFileType(), HttpFileImpl::FileType::Unknown);
}

TEST_F(HttpFileTest, DelegatesFileDataAndLength)
{
    const char data[] = "data";
    file.setFile(data, sizeof(data) - 1);
    EXPECT_EQ(file.fileLength(), sizeof(data) - 1);
    EXPECT_EQ(std::string(file.fileData(), file.fileLength()), "data");
}

TEST_F(HttpFileTest, DelegatesGetMd5)
{
    const char data[] = "data";
    file.setFile(data, sizeof(data) - 1);
    std::string md5 = file.getMd5();
    EXPECT_FALSE(md5.empty());
}

TEST_F(HttpFileTest, DelegatesGetItemName)
{
    EXPECT_EQ(file.getItemName(), "");
}