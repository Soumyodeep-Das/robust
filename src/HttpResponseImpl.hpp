#ifndef HTTPRESPONSEIMPL_HPP
#define HTTPRESPONSEIMPL_HPP

#include <memory>
#include <string>
#include <string_view>
#include <map>
#include <mutex>
#include <json/json.h>
#include <trantor/net/MsgBuffer.h>
#include <drogon/HttpResponse.h>
#include <drogon/Cookie.h>

namespace drogon
{
class HttpResponseImpl;
using HttpResponseImplPtr = std::shared_ptr<HttpResponseImpl>;

class HttpResponseImpl : public HttpResponse
{
  public:
    const char *versionString() const;

    void generateBodyFromJson() const;

    void makeHeaderString(trantor::MsgBuffer &buffer);

    void renderToBuffer(trantor::MsgBuffer &buffer);

    std::shared_ptr<trantor::MsgBuffer> renderToBuffer();

    std::shared_ptr<trantor::MsgBuffer> renderHeaderForHeadMethod();

    void addHeader(const char *start, const char *colon, const char *end);

    void swap(HttpResponseImpl &that) noexcept;

    void clear();

    void parseJson() const;

    bool shouldBeCompressed() const;

    void setContentTypeString(const char *typeString, size_t typeStringLength);

  private:
    // Add private members here if needed
};

}  // namespace drogon

#endif  // HTTPRESPONSEIMPL_HPP