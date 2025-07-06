#ifndef RANGETESTCONTROLLER_HPP
#define RANGETESTCONTROLLER_HPP

#include <cstddef>
#include <functional>

class HttpRequestPtr;
class HttpResponsePtr;
class HttpResponse;

class RangeTestController
{
public:
    RangeTestController();

    void getFile(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback) const;

    void getFileByRange(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback,
        size_t offset,
        size_t length) const;

private:
    static size_t fileSize_;
};

#endif // RANGETESTCONTROLLER_HPP