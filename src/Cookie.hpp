#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <optional>
#include <cstdint>

namespace drogon
{
class Date;

enum class SameSite
{
    kNull,
    kLax,
    kStrict,
    kNone
};

class Cookie
{
  public:
    std::string cookieString() const;

  private:
    std::string key_;
    std::string value_;
    Date expiresDate_;
    std::optional<int> maxAge_;
    std::string domain_;
    std::string path_;
    SameSite sameSite_{SameSite::kNull};
    bool secure_{false};
    bool httpOnly_{false};
};

}  // namespace drogon

#endif  // COOKIE_HPP