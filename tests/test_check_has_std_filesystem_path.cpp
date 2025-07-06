#include "gtest/gtest.h"
#include "check_has_std_filesystem_path.hpp"

TEST(CheckHasStdFilesystemPath, CreatePathReturnsSamePath) {
    std::filesystem::path testPath("../");
    std::filesystem::path result = std_fs_check::createPath(testPath);
    EXPECT_EQ(result, testPath);
}