#include "file_upload.hpp"
#include <drogon/drogon.h>
using namespace drogon;

void registerHandlers()
{
    app().registerHandler(
        "/",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpViewResponse("FileUpload");
            callback(resp);
        });

    app().registerHandler(
        "/upload_endpoint",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            MultiPartParser fileUpload;
            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setBody("Must only be one file");
                resp->setStatusCode(k403Forbidden);
                callback(resp);
                return;
            }

            auto &file = fileUpload.getFiles()[0];
            auto md5 = file.getMd5();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(
                "The server has calculated the file's MD5 hash to be " + md5);
            file.save();
            LOG_INFO << "The uploaded file has been saved to the ./uploads "
                        "directory";
            callback(resp);
        },
        {Post});
}