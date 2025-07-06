#include "SharedLibManager.hpp"
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace drogon;

class SharedLibManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code if needed
    }
    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(SharedLibManagerTest, ShouldCompileLibReturnsTrueIfSoFileMissing)
{
    SharedLibManager manager({}, "");
    struct stat sourceStat{};
#if defined __linux__ || defined __HAIKU__
    sourceStat.st_mtim.tv_sec = 100;
#elif defined _WIN32
    sourceStat.st_mtime = 100;
#else
    sourceStat.st_mtimespec.tv_sec = 100;
#endif

    // Provide a non-existent soFile path
    std::string soFile = "/nonexistent/path/lib.so";

    EXPECT_TRUE(manager.shouldCompileLib(soFile, sourceStat));
}

TEST_F(SharedLibManagerTest, ShouldCompileLibReturnsTrueIfSourceNewer)
{
    SharedLibManager manager({}, "");
    struct stat sourceStat{};
    struct stat soStat{};

#if defined __linux__ || defined __HAIKU__
    sourceStat.st_mtim.tv_sec = 200;
    soStat.st_mtim.tv_sec = 100;
#elif defined _WIN32
    sourceStat.st_mtime = 200;
    soStat.st_mtime = 100;
#else
    sourceStat.st_mtimespec.tv_sec = 200;
    soStat.st_mtimespec.tv_sec = 100;
#endif

    // Create a temporary soFile
    std::string soFile = "temp_lib.so";

    // Create empty soFile to pass stat
    FILE *fp = fopen(soFile.c_str(), "w");
    ASSERT_NE(fp, nullptr);
    fclose(fp);

    // Modify soFile time to soStat time
#if defined __linux__ || defined __HAIKU__
    struct timespec times[2];
    times[0] = soStat.st_mtim;
    times[1] = soStat.st_mtim;
    utimensat(AT_FDCWD, soFile.c_str(), times, 0);
#elif defined _WIN32
    // Windows: skipping time modification for simplicity
#else
    struct timespec times[2];
    times[0] = soStat.st_mtimespec;
    times[1] = soStat.st_mtimespec;
    utimensat(AT_FDCWD, soFile.c_str(), times, 0);
#endif

    EXPECT_TRUE(manager.shouldCompileLib(soFile, sourceStat));

    // Cleanup
    remove(soFile.c_str());
}

TEST_F(SharedLibManagerTest, ShouldCompileLibReturnsFalseIfSoNewer)
{
    SharedLibManager manager({}, "");
    struct stat sourceStat{};
    struct stat soStat{};

#if defined __linux__ || defined __HAIKU__
    sourceStat.st_mtim.tv_sec = 100;
    soStat.st_mtim.tv_sec = 200;
#elif defined _WIN32
    sourceStat.st_mtime = 100;
    soStat.st_mtime = 200;
#else
    sourceStat.st_mtimespec.tv_sec = 100;
    soStat.st_mtimespec.tv_sec = 200;
#endif

    // Create a temporary soFile
    std::string soFile = "temp_lib.so";

    // Create empty soFile to pass stat
    FILE *fp = fopen(soFile.c_str(), "w");
    ASSERT_NE(fp, nullptr);
    fclose(fp);

    // Modify soFile time to soStat time
#if defined __linux__ || defined __HAIKU__
    struct timespec times[2];
    times[0] = soStat.st_mtim;
    times[1] = soStat.st_mtim;
    utimensat(AT_FDCWD, soFile.c_str(), times, 0);
#elif defined _WIN32
    // Windows: skipping time modification for simplicity
#else
    struct timespec times[2];
    times[0] = soStat.st_mtimespec;
    times[1] = soStat.st_mtimespec;
    utimensat(AT_FDCWD, soFile.c_str(), times, 0);
#endif

    EXPECT_FALSE(manager.shouldCompileLib(soFile, sourceStat));

    // Cleanup
    remove(soFile.c_str());
}

TEST_F(SharedLibManagerTest, LoadLibReturnsNullptrOnInvalidFile)
{
    SharedLibManager manager({}, "");
    void *handle = manager.loadLib("/invalid/path/to/lib.so", nullptr);
    EXPECT_EQ(handle, nullptr);
}

TEST_F(SharedLibManagerTest, CompileAndLoadLibReturnsNullptrOnFail)
{
    SharedLibManager manager({}, "");
    // Provide an invalid source file path to force compile failure
    void *handle = manager.compileAndLoadLib("/invalid/source/file.cc", nullptr);
    EXPECT_EQ(handle, nullptr);
}

TEST_F(SharedLibManagerTest, ManagerLibsDoesNotThrow)
{
    std::vector<std::string> paths = {"./"};
    SharedLibManager manager(paths, "");
    EXPECT_NO_THROW(manager.managerLibs());
}