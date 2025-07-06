#ifndef BENCHMARKCTRL_HPP
#define BENCHMARKCTRL_HPP

#include <functional>

class HttpRequestPtr;
class HttpResponsePtr;
class HttpResponse;

class BenchmarkCtrl {
public:
    void asyncHandleHttpRequest(
        const HttpRequestPtr &,
        std::function<void(const HttpResponsePtr &)> &&callback);
};

#endif // BENCHMARKCTRL_HPP