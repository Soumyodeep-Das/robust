#include "LocalHostFilter.hpp"
#include "HttpResponseImpl.h"

using namespace drogon;

void LocalHostFilter::doFilter(const HttpRequestPtr &req,
                               FilterCallback &&fcb,
                               FilterChainCallback &&fccb)
{
    if (req->peerAddr().isLoopbackIp())
    {
        fccb();
        return;
    }
    auto res = drogon::HttpResponse::newNotFoundResponse();
    fcb(res);
}