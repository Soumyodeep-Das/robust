#ifndef API_ATTACHMENT_HPP
#define API_ATTACHMENT_HPP

#include <functional>
#include <string>
#include "api_Attachment.h" // Assuming this is needed for HttpRequestPtr, HttpResponsePtr, etc.

namespace api {

class Attachment {
public:
    void get(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback);

    void upload(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void uploadImage(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback);

    void download(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
};

} // namespace api

#endif // API_ATTACHMENT_HPP