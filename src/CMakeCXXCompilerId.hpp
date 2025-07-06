#ifndef CMAKECXXCOMPILERID_HPP
#define CMAKECXXCOMPILERID_HPP

extern const char* info_compiler;
#ifdef SIMULATE_ID
extern const char* info_simulate;
#endif

#ifdef __QNXNTO__
extern const char* qnxnto;
#endif

#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
extern const char* info_cray;
#endif

extern const char* info_platform;
extern const char* info_arch;

#ifdef COMPILER_VERSION
extern const char* info_version;
#elif defined(COMPILER_VERSION_MAJOR)
extern const char info_version[];
#endif

#ifdef COMPILER_VERSION_INTERNAL
extern const char info_version_internal[];
#elif defined(COMPILER_VERSION_INTERNAL_STR)
extern const char* info_version_internal;
#endif

#ifdef SIMULATE_VERSION_MAJOR
extern const char info_simulate_version[];
#endif

extern const char* info_language_standard_default;
extern const char* info_language_extensions_default;

#endif // CMAKECXXCOMPILERID_HPP