#include "CookieTest.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace drogon
{

Cookie::Cookie(const std::string &name, const std::string &value)
    : _name(name), _value(value)
{
}

void Cookie::setSecure(bool secure)
{
    _secure = secure;
}

void Cookie::setDomain(const std::string &domain)
{
    _domain = domain;
}

void Cookie::setExpiresDate(long long microsecondsSinceEpoch)
{
    _expiresDate = microsecondsSinceEpoch;
}

void Cookie::setMaxAge(int maxAge)
{
    _maxAge = maxAge;
}

void Cookie::setSameSite(SameSite sameSite)
{
    _sameSite = sameSite;
}

std::string Cookie::cookieString() const
{
    std::ostringstream oss;
    oss << "Set-Cookie: " << _name << "=" << _value;

    if (_expiresDate > 0)
    {
        // Convert microseconds since epoch to time_t seconds
        std::time_t t = static_cast<std::time_t>(_expiresDate / 1000000);
        std::tm gm = *std::gmtime(&t);
        oss << "; Expires=" << std::put_time(&gm, "%a, %d %b %Y %H:%M:%S GMT");
    }

    if (!_domain.empty())
    {
        oss << "; Domain=" << _domain;
    }

    if (_maxAge > 0)
    {
        oss << "; Max-Age=" << _maxAge;
    }

    if (_sameSite == SameSite::kStrict)
    {
        oss << "; SameSite=Strict";
    }
    else if (_sameSite == SameSite::kNone)
    {
        oss << "; SameSite=None";
    }

    if (_secure || _sameSite == SameSite::kNone)
    {
        oss << "; Secure";
    }

    oss << "; HttpOnly\r\n";

    return oss.str();
}

} // namespace drogon