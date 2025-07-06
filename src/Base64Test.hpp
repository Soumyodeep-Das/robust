#ifndef BASE64TEST_HPP
#define BASE64TEST_HPP

#include <string>

namespace drogon
{
namespace utils
{
std::string base64Encode(const unsigned char *data, size_t len, bool urlSafe = false);
std::string base64Decode(const std::string &encoded);
}  // namespace utils
}  // namespace drogon

#endif  // BASE64TEST_HPP