#include "api_Attachment.hpp"
#include <fstream>

using namespace api;

void Attachment::get(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpViewResponse("FileUpload", HttpViewData());
    callback(resp);
}

void Attachment::upload(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback)
{
    MultiPartParser fileUpload;
    if (fileUpload.parse(req) == 0)
    {
        auto &files = fileUpload.getFiles();
        for (auto const &file : files)
        {
            LOG_DEBUG << "file:" << file.getFileName()
                      << "(extension=" << file.getFileExtension()
                      << ",type=" << file.getFileType()
                      << ",len=" << file.fileLength()
                      << ",md5=" << file.getMd5() << ")";
            file.save();
            file.save("123");
            file.saveAs("456/hehe");
            file.saveAs("456/7/8/9/" + file.getMd5());
            file.save("..");
            file.save(".xx");
            file.saveAs("../xxx");
        }
        Json::Value json;
        json["result"] = "ok";
        for (auto &param : fileUpload.getParameters())
        {
            json[param.first] = param.second;
        }
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    LOG_DEBUG << "upload error!";
    Json::Value json;
    json["result"] = "failed";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void Attachment::uploadImage(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    MultiPartParser fileUpload;

    if (fileUpload.parse(req) == 0 && fileUpload.getFiles().size() == 1)
    {
        Json::Value json;
        auto &file = fileUpload.getFiles()[0];

        if (file.getFileType() == FT_IMAGE)
        {
            json["isImage"] = true;
        }
        auto fileExtension = file.getFileExtension();
        if (fileExtension == "png" || fileExtension == "jpeg" ||
            fileExtension == "jpg" || fileExtension == "ico")
        {
            json["isImage"] = true;
        }
        else
        {
            json["isImage"] = false;
        }

        json["result"] = "ok";
        for (auto &param : fileUpload.getParameters())
        {
            json[param.first] = param.second;
        }
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    LOG_DEBUG << "upload image error!";
    Json::Value json;
    json["result"] = "failed";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void Attachment::download(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newFileResponse("./drogon.jpg", "", CT_IMAGE_JPG);
    callback(resp);
}