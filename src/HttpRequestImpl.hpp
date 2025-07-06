#ifndef HTTPREQUESTIMPL_HPP
#define HTTPREQUESTIMPL_HPP

#include <string>
#include <string_view>
#include <memory>
#include <map>
#include <vector>
#include <mutex>
#include <json/json.h>
#include <trantor/net/MsgBuffer.h>
#include "HttpRequest.h"
#include "UploadFile.h"

namespace drogon
{
class HttpFileUploadRequest;

class HttpRequestImpl : public HttpRequest
{
  public:
    void parseJson() const;
    void parseParameters() const;
    void appendToBuffer(trantor::MsgBuffer *output) const;
    void addHeader(const char *start, const char *colon, const char *end);
    void swap(HttpRequestImpl &that) noexcept;
    const char *versionString() const;
    const char *methodString() const;
    bool setMethod(const char *start, const char *end);
    ~HttpRequestImpl() override;
    void reserveBodySize(size_t length);
    void appendToBody(const char *data, size_t length);
    void createTmpFile();
    void setContentTypeString(const char *typeString, size_t typeStringLength);

  private:
    // Private members are not declared here as per instruction to extract public interface only
};

}  // namespace drogon

#endif  // HTTPREQUESTIMPL_HPP