#include "HttpUtils.hpp"
#include <gtest/gtest.h>

namespace drogon
{

TEST(HttpUtilsTest, StatusCodeToStringKnownCodes)
{
    EXPECT_EQ(statusCodeToString(200), "OK");
    EXPECT_EQ(statusCodeToString(404), "Not Found");
    EXPECT_EQ(statusCodeToString(500), "Internal Server Error");
    EXPECT_EQ(statusCodeToString(418), "I'm a Teapot");
}

TEST(HttpUtilsTest, StatusCodeToStringUnknownCodes)
{
    EXPECT_EQ(statusCodeToString(150), "Informational");
    EXPECT_EQ(statusCodeToString(250), "Successful");
    EXPECT_EQ(statusCodeToString(350), "Redirection");
    EXPECT_EQ(statusCodeToString(450), "Bad Request");
    EXPECT_EQ(statusCodeToString(550), "Server Error");
    EXPECT_EQ(statusCodeToString(700), "Undefined Error");
}

TEST(HttpUtilsTest, GetContentTypeKnownExtensions)
{
    EXPECT_EQ(getContentType("file.js"), CT_APPLICATION_X_JAVASCRIPT);
    EXPECT_EQ(getContentType("file.bmp"), CT_IMAGE_BMP);
    EXPECT_EQ(getContentType("file.css"), CT_TEXT_CSS);
    EXPECT_EQ(getContentType("file.eot"), CT_APPLICATION_VND_MS_FONTOBJ);
    EXPECT_EQ(getContentType("file.gif"), CT_IMAGE_GIF);
    EXPECT_EQ(getContentType("file.ico"), CT_IMAGE_XICON);
    EXPECT_EQ(getContentType("file.jpg"), CT_IMAGE_JPG);
    EXPECT_EQ(getContentType("file.otf"), CT_APPLICATION_X_FONT_OPENTYPE);
    EXPECT_EQ(getContentType("file.png"), CT_IMAGE_PNG);
    EXPECT_EQ(getContentType("file.pdf"), CT_APPLICATION_PDF);
    EXPECT_EQ(getContentType("file.svg"), CT_IMAGE_SVG_XML);
    EXPECT_EQ(getContentType("file.txt"), CT_TEXT_PLAIN);
    EXPECT_EQ(getContentType("file.xml"), CT_TEXT_XML);
    EXPECT_EQ(getContentType("file.xsl"), CT_TEXT_XSL);
    EXPECT_EQ(getContentType("file.avif"), CT_IMAGE_AVIF);
    EXPECT_EQ(getContentType("file.html"), CT_TEXT_HTML);
    EXPECT_EQ(getContentType("file.jpeg"), CT_IMAGE_JPG);
    EXPECT_EQ(getContentType("file.icns"), CT_IMAGE_ICNS);
    EXPECT_EQ(getContentType("file.webp"), CT_IMAGE_WEBP);
    EXPECT_EQ(getContentType("file.wasm"), CT_APPLICATION_WASM);
    EXPECT_EQ(getContentType("file.woff"), CT_APPLICATION_FONT_WOFF);
    EXPECT_EQ(getContentType("file.woff2"), CT_APPLICATION_FONT_WOFF2);
}

TEST(HttpUtilsTest, GetContentTypeUnknownExtension)
{
    EXPECT_EQ(getContentType("file.unknownext"), CT_APPLICATION_OCTET_STREAM);
    EXPECT_EQ(getContentType("file"), CT_APPLICATION_OCTET_STREAM);
}

TEST(HttpUtilsTest, ParseContentTypeKnown)
{
    EXPECT_EQ(parseContentType("text/html"), CT_TEXT_HTML);
    EXPECT_EQ(parseContentType("application/json"), CT_APPLICATION_JSON);
    EXPECT_EQ(parseContentType("image/png"), CT_IMAGE_PNG);
    EXPECT_EQ(parseContentType("multipart/form-data"), CT_MULTIPART_FORM_DATA);
}

TEST(HttpUtilsTest, ParseContentTypeUnknown)
{
    EXPECT_EQ(parseContentType("unknown/type"), CT_NONE);
    EXPECT_EQ(parseContentType(""), CT_NONE);
}

TEST(HttpUtilsTest, ParseFileTypeKnown)
{
    EXPECT_EQ(parseFileType("html"), FT_DOCUMENT);
    EXPECT_EQ(parseFileType("docx"), FT_DOCUMENT);
    EXPECT_EQ(parseFileType("zip"), FT_ARCHIVE);
    EXPECT_EQ(parseFileType("mp3"), FT_AUDIO);
    EXPECT_EQ(parseFileType("avi"), FT_MEDIA);
    EXPECT_EQ(parseFileType("gif"), FT_MEDIA);
    EXPECT_EQ(parseFileType("png"), FT_IMAGE);
    EXPECT_EQ(parseFileType("jpg"), FT_IMAGE);
    EXPECT_EQ(parseFileType("ico"), FT_IMAGE);
}

TEST(HttpUtilsTest, ParseFileTypeUnknown)
{
    EXPECT_EQ(parseFileType("unknownext"), FT_CUSTOM);
    EXPECT_EQ(parseFileType(""), FT_UNKNOWN);
}

TEST(HttpUtilsTest, ContentTypeToMimeKnown)
{
    EXPECT_EQ(contentTypeToMime(CT_TEXT_HTML), "text/html; charset=utf-8");
    EXPECT_EQ(contentTypeToMime(CT_APPLICATION_JSON), "application/json; charset=utf-8");
    EXPECT_EQ(contentTypeToMime(CT_IMAGE_PNG), "image/png");
    EXPECT_EQ(contentTypeToMime(CT_NONE), "");
    EXPECT_EQ(contentTypeToMime(CT_TEXT_PLAIN), "text/plain; charset=utf-8");
}

TEST(HttpUtilsTest, RegisterCustomExtensionMimeAndLookup)
{
    const std::string ext = "customext";
    const std::string mime = "application/custom-mime";

    registerCustomExtensionMime(ext, mime);

    EXPECT_EQ(fileNameToMime("file." + ext), mime);
    auto pair = fileNameToContentTypeAndMime("file." + ext);
    EXPECT_EQ(pair.first, CT_CUSTOM);
    EXPECT_EQ(pair.second, mime);
}

TEST(HttpUtilsTest, FileNameToMimeKnownAndUnknown)
{
    EXPECT_EQ(fileNameToMime("file.html"), "text/html; charset=utf-8");
    EXPECT_EQ(fileNameToMime("file.unknownext"), "");
    EXPECT_EQ(fileNameToMime("file"), "");
}

TEST(HttpUtilsTest, FileNameToContentTypeAndMimeKnown)
{
    auto pair = fileNameToContentTypeAndMime("file.html");
    EXPECT_EQ(pair.first, CT_TEXT_HTML);
    EXPECT_EQ(pair.second, "text/html; charset=utf-8");

    pair = fileNameToContentTypeAndMime("file.unknownext");
    EXPECT_EQ(pair.first, CT_NONE);
    EXPECT_EQ(pair.second, "");
}

}  // namespace drogon