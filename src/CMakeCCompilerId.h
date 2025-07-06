#ifndef CMAKE_C_COMPILER_ID_H
#define CMAKE_C_COMPILER_ID_H

extern char const* info_compiler;
#ifdef SIMULATE_ID
extern char const* info_simulate;
#endif

#ifdef __QNXNTO__
extern char const* qnxnto;
#endif

#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
extern char const *info_cray;
#endif

extern char const* info_platform;
extern char const* info_arch;

#ifdef COMPILER_VERSION
extern char const* info_version;
#elif defined(COMPILER_VERSION_MAJOR)
extern char const info_version[];
#endif

#ifdef COMPILER_VERSION_INTERNAL
extern char const info_version_internal[];
#elif defined(COMPILER_VERSION_INTERNAL_STR)
extern char const* info_version_internal;
#endif

#ifdef SIMULATE_VERSION_MAJOR
extern char const info_simulate_version[];
#endif

extern const char* info_language_standard_default;
extern const char* info_language_extensions_default;

#endif /* CMAKE_C_COMPILER_ID_H */