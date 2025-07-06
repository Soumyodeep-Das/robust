#ifndef BEGINADVICETEST_HPP
#define BEGINADVICETEST_HPP

#include <string>
#include <functional>
#include "HttpRequest.h"
#include "HttpResponse.h"

class BeginAdviceTest {
public:
    static std::string content_;

    void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // BEGINADVICETEST_HPP