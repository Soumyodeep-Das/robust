#include "gtest/gtest.h"
#include "CMakeCXXCompilerId.hpp"

TEST(CMakeCXXCompilerIdTest, InfoCompilerNotNull) {
    ASSERT_NE(info_compiler, nullptr);
    ASSERT_STRNE(info_compiler, "");
}

#ifdef SIMULATE_ID
TEST(CMakeCXXCompilerIdTest, InfoSimulateNotNull) {
    ASSERT_NE(info_simulate, nullptr);
    ASSERT_STRNE(info_simulate, "");
}
#endif

#ifdef __QNXNTO__
TEST(CMakeCXXCompilerIdTest, QnxntoNotNull) {
    ASSERT_NE(qnxnto, nullptr);
    ASSERT_STRNE(qnxnto, "");
}
#endif

#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
TEST(CMakeCXXCompilerIdTest, InfoCrayNotNull) {
    ASSERT_NE(info_cray, nullptr);
    ASSERT_STRNE(info_cray, "");
}
#endif

TEST(CMakeCXXCompilerIdTest, InfoPlatformNotNull) {
    ASSERT_NE(info_platform, nullptr);
    ASSERT_STRNE(info_platform, "");
}

TEST(CMakeCXXCompilerIdTest, InfoArchNotNull) {
    ASSERT_NE(info_arch, nullptr);
    ASSERT_STRNE(info_arch, "");
}

#ifdef COMPILER_VERSION
TEST(CMakeCXXCompilerIdTest, InfoVersionNotNull) {
    ASSERT_NE(info_version, nullptr);
    ASSERT_STRNE(info_version, "");
}
#elif defined(COMPILER_VERSION_MAJOR)
TEST(CMakeCXXCompilerIdTest, InfoVersionArrayNotEmpty) {
    ASSERT_GT(sizeof(info_version), 0);
    ASSERT_EQ(info_version[sizeof(info_version)-1], '\0');
}
#endif

#ifdef COMPILER_VERSION_INTERNAL
TEST(CMakeCXXCompilerIdTest, InfoVersionInternalNotEmpty) {
    ASSERT_GT(sizeof(info_version_internal), 0);
    ASSERT_EQ(info_version_internal[sizeof(info_version_internal)-1], '\0');
}
#elif defined(COMPILER_VERSION_INTERNAL_STR)
TEST(CMakeCXXCompilerIdTest, InfoVersionInternalStrNotNull) {
    ASSERT_NE(info_version_internal, nullptr);
    ASSERT_STRNE(info_version_internal, "");
}
#endif

#ifdef SIMULATE_VERSION_MAJOR
TEST(CMakeCXXCompilerIdTest, InfoSimulateVersionNotEmpty) {
    ASSERT_GT(sizeof(info_simulate_version), 0);
    ASSERT_EQ(info_simulate_version[sizeof(info_simulate_version)-1], '\0');
}
#endif

TEST(CMakeCXXCompilerIdTest, InfoLanguageStandardDefaultNotNull) {
    ASSERT_NE(info_language_standard_default, nullptr);
    ASSERT_STRNE(info_language_standard_default, "");
}

TEST(CMakeCXXCompilerIdTest, InfoLanguageExtensionsDefaultNotNull) {
    ASSERT_NE(info_language_extensions_default, nullptr);
    ASSERT_STRNE(info_language_extensions_default, "");
}