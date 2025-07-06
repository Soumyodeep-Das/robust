#ifndef TESTCONTROLLER_HPP
#define TESTCONTROLLER_HPP

#include <functional>
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace example {

class TestController {
public:
    void asyncHandleHttpRequest(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

} // namespace example

#endif // TESTCONTROLLER_HPP