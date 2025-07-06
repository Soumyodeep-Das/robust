#ifndef CHECK_HAS_STD_FILESYSTEM_PATH_HPP
#define CHECK_HAS_STD_FILESYSTEM_PATH_HPP

#include <filesystem>

namespace std_fs_check {
    std::filesystem::path createPath(const std::filesystem::path& p);
}

#endif // CHECK_HAS_STD_FILESYSTEM_PATH_HPP