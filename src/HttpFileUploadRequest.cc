#include "HttpFileUploadRequest.hpp"
#include <drogon/utils/Utilities.h>

using namespace drogon;

HttpFileUploadRequest::HttpFileUploadRequest(
    const std::vector<UploadFile> &files)
    : HttpRequestImpl(nullptr),
      boundary_(utils::genRandomString(32)),
      files_(files)
{
    setMethod(drogon::Post);
    setVersion(drogon::Version::kHttp11);
    setContentType("multipart/form-data; boundary=" + boundary_);
    contentType_ = CT_MULTIPART_FORM_DATA;
}