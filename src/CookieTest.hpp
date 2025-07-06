#ifndef COOKIE_TEST_HPP
#define COOKIE_TEST_HPP

#include <string>

namespace drogon
{
class Cookie
{
public:
    enum class SameSite
    {
        kStrict,
        kNone
    };

    Cookie(const std::string &name, const std::string &value);

    void setSecure(bool secure);
    void setDomain(const std::string &domain);
    void setExpiresDate(long long microsecondsSinceEpoch);
    void setMaxAge(int maxAge);
    void setSameSite(SameSite sameSite);

    std::string cookieString() const;

private:
    std::string _name;
    std::string _value;
    bool _secure{false};
    std::string _domain;
    long long _expiresDate{0};
    int _maxAge{0};
    SameSite _sameSite{SameSite::kStrict};
};

} // namespace drogon

#endif // COOKIE_TEST_HPP