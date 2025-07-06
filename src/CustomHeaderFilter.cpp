#include "CustomHeaderFilter.hpp"

using namespace drogon;

void CustomHeaderFilter::doFilter(const HttpRequestPtr &req,
                                  std::function<void(const HttpResponsePtr &)> &&fcb,
                                  std::function<void()> &&fccb)
{
    if (req->getHeader(field_) == value_)
    {
        // Passed
        fccb();
        return;
    }
    // Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}