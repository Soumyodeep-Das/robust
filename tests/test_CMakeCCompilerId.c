#include "unity/unity.h"
#include "CMakeCCompilerId.h"

void setUp(void) {}
void tearDown(void) {}

void test_info_compiler_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_compiler);
}

#ifdef SIMULATE_ID
void test_info_simulate_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_simulate);
}
#endif

#ifdef __QNXNTO__
void test_qnxnto_not_null(void) {
    TEST_ASSERT_NOT_NULL(qnxnto);
}
#endif

#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
void test_info_cray_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_cray);
}
#endif

void test_info_platform_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_platform);
}

void test_info_arch_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_arch);
}

#ifdef COMPILER_VERSION
void test_info_version_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_version);
}
#elif defined(COMPILER_VERSION_MAJOR)
void test_info_version_array_not_empty(void) {
    TEST_ASSERT_EQUAL_CHAR('I', info_version[0]);
    TEST_ASSERT_EQUAL_CHAR('N', info_version[1]);
    TEST_ASSERT_EQUAL_CHAR('F', info_version[2]);
}
#endif

#ifdef COMPILER_VERSION_INTERNAL
void test_info_version_internal_not_empty(void) {
    TEST_ASSERT_EQUAL_CHAR('I', info_version_internal[0]);
    TEST_ASSERT_EQUAL_CHAR('N', info_version_internal[1]);
    TEST_ASSERT_EQUAL_CHAR('F', info_version_internal[2]);
}
#elif defined(COMPILER_VERSION_INTERNAL_STR)
void test_info_version_internal_str_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_version_internal);
}
#endif

#ifdef SIMULATE_VERSION_MAJOR
void test_info_simulate_version_not_empty(void) {
    TEST_ASSERT_EQUAL_CHAR('I', info_simulate_version[0]);
    TEST_ASSERT_EQUAL_CHAR('N', info_simulate_version[1]);
    TEST_ASSERT_EQUAL_CHAR('F', info_simulate_version[2]);
}
#endif

void test_info_language_standard_default_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_language_standard_default);
}

void test_info_language_extensions_default_not_null(void) {
    TEST_ASSERT_NOT_NULL(info_language_extensions_default);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_info_compiler_not_null);
#ifdef SIMULATE_ID
    RUN_TEST(test_info_simulate_not_null);
#endif
#ifdef __QNXNTO__
    RUN_TEST(test_qnxnto_not_null);
#endif
#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
    RUN_TEST(test_info_cray_not_null);
#endif
    RUN_TEST(test_info_platform_not_null);
    RUN_TEST(test_info_arch_not_null);
#ifdef COMPILER_VERSION
    RUN_TEST(test_info_version_not_null);
#elif defined(COMPILER_VERSION_MAJOR)
    RUN_TEST(test_info_version_array_not_empty);
#endif
#ifdef COMPILER_VERSION_INTERNAL
    RUN_TEST(test_info_version_internal_not_empty);
#elif defined(COMPILER_VERSION_INTERNAL_STR)
    RUN_TEST(test_info_version_internal_str_not_null);
#endif
#ifdef SIMULATE_VERSION_MAJOR
    RUN_TEST(test_info_simulate_version_not_empty);
#endif
    RUN_TEST(test_info_language_standard_default_not_null);
    RUN_TEST(test_info_language_extensions_default_not_null);
    return UNITY_END();
}