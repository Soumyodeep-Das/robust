#ifndef HTTPFILEUPLOADREQUEST_HPP
#define HTTPFILEUPLOADREQUEST_HPP

#include <vector>
#include <string>
#include "HttpRequestImpl.h"
#include "drogon/UploadFile.h"

namespace drogon
{
class HttpFileUploadRequest : public HttpRequestImpl
{
  public:
    explicit HttpFileUploadRequest(const std::vector<UploadFile> &files);

  private:
    std::string boundary_;
    std::vector<UploadFile> files_;
};
}  // namespace drogon

#endif  // HTTPFILEUPLOADREQUEST_HPP