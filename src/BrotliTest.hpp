#ifndef BROTLI_TEST_HPP
#define BROTLI_TEST_HPP

#include <string>
#include <vector>

namespace drogon
{
namespace utils
{
    std::vector<unsigned char> brotliCompress(const char* data, size_t length);
    std::string brotliDecompress(const unsigned char* data, size_t length);
}
}

#endif // BROTLI_TEST_HPP