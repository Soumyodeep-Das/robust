#include "LoginFilter.hpp"

void LoginFilter::doFilter(const drogon::HttpRequestPtr &req,
                           std::function<void(const drogon::HttpResponsePtr &)> &&fcb,
                           std::function<void()> &&fccb)
{
    // Implementation should remain here or be added by user.
}