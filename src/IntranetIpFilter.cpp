#include "IntranetIpFilter.hpp"
#include "HttpResponseImpl.h"
#include <drogon/IntranetIpFilter.h>

using namespace drogon;

void IntranetIpFilter::doFilter(const HttpRequestPtr &req,
                                FilterCallback &&fcb,
                                FilterChainCallback &&fccb)
{
    if (req->peerAddr().isIntranetIp())
    {
        fccb();
        return;
    }
    auto res = drogon::HttpResponse::newNotFoundResponse();
    fcb(res);
}