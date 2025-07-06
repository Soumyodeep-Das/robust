#include "gtest/gtest.h"
#include "create_view.h"

namespace drogon_ctl
{

class CreateViewTest : public ::testing::Test
{
protected:
    create_view view;

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CreateViewTest, HandleCommand_EmptyParameters)
{
    std::vector<std::string> params;
    // Should not throw or crash
    view.handleCommand(params);
    SUCCEED();
}

TEST_F(CreateViewTest, CreateViewFile_InvalidFile)
{
    int ret = view.createViewFile("nonexistent_file.txt");
    EXPECT_EQ(ret, -1);
}

TEST_F(CreateViewTest, CreateViewFile_ValidFile)
{
    // Create a temporary file with minimal content
    const char *filename = "test_template.csp";
    std::ofstream ofs(filename);
    ofs << "<%layout testLayout %>\nHello World\n";
    ofs.close();

    view.handleCommand(std::vector<std::string>{filename, "-o", "."});
    // The output files should be created in current directory, but we do not check file system here.
    // Just test createViewFile returns 0 for valid file
    int ret = view.createViewFile(filename);
    EXPECT_EQ(ret, 0);

    std::remove(filename);
}

TEST_F(CreateViewTest, CreateViewFiles_MultipleFiles)
{
    const char *filename1 = "test1.csp";
    const char *filename2 = "test2.csp";
    std::ofstream ofs1(filename1);
    ofs1 << "Hello 1\n";
    ofs1.close();
    std::ofstream ofs2(filename2);
    ofs2 << "Hello 2\n";
    ofs2.close();

    std::vector<std::string> files{filename1, filename2};
    // This will call exit(1) if any file creation fails, so we expect no exceptions here
    // but since exit(1) terminates process, we cannot test that easily.
    // So just test createViewFiles calls createViewFile for each file.
    // We can test createViewFile separately.
    // Here we just call createViewFiles and expect no exceptions.
    // Set output path to current directory
    view.handleCommand(std::vector<std::string>{"-o", "."});
    view.createViewFiles(files);

    std::remove(filename1);
    std::remove(filename2);
}

}  // namespace drogon_ctl