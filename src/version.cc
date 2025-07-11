#include "version.hpp"
#include <drogon/config.h>
#include <drogon/version.h>
#include <trantor/net/Resolver.h>
#include <iostream>

using namespace drogon_ctl;
static const char banner[] =
    "     _                             \n"
    "  __| |_ __ ___   __ _  ___  _ __  \n"
    " / _` | '__/ _ \\ / _` |/ _ \\| '_ \\ \n"
    "| (_| | | | (_) | (_| | (_) | | | |\n"
    " \\__,_|_|  \\___/ \\__, |\\___/|_| |_|\n"
    "                 |___/             \n";

void version::handleCommand(std::vector<std::string> &parameters)
{
    std::cout << banner << std::endl;
    std::cout << "A utility for drogon" << std::endl;
    std::cout << "Version: " << DROGON_VERSION << std::endl;
    std::cout << "Git commit: " << DROGON_VERSION_SHA1 << std::endl;
    std::cout << "Compilation: \n  Compiler: " << COMPILER_COMMAND
              << "\n  Compiler ID: " << COMPILER_ID
              << "\n  Compilation flags: " << COMPILATION_FLAGS
              << INCLUDING_DIRS << std::endl;
    std::cout << "Libraries: \n  postgresql: "
              << (USE_POSTGRESQL ? "yes" : "no") << "  (batch mode: "
              << (LIBPQ_SUPPORTS_BATCH_MODE ? "yes)\n" : "no)\n")
              << "  mariadb: " << (USE_MYSQL ? "yes\n" : "no\n")
              << "  sqlite3: " << (USE_SQLITE3 ? "yes\n" : "no\n");
#ifdef OpenSSL_FOUND
    std::cout << "  openssl: yes\n";
#else
    std::cout << "  openssl: no\n";
#endif
#ifdef USE_BROTLI
    std::cout << "  brotli: yes\n";
#else
    std::cout << "  brotli: no\n";
#endif
#ifdef Boost_FOUND
    std::cout << "  boost: yes\n";
#else
    std::cout << "  boost: no\n";
#endif
#ifdef USE_REDIS
    std::cout << "  hiredis: yes\n";
#else
    std::cout << "  hiredis: no\n";
#endif
    std::cout << "  c-ares: "
              << (trantor::Resolver::isCAresUsed() ? "yes\n" : "no\n");
}