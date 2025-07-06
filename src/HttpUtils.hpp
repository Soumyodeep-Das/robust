#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace drogon
{
enum ContentType
{
    CT_NONE,
    CT_TEXT_HTML,
    CT_APPLICATION_X_FORM,
    CT_APPLICATION_XML,
    CT_APPLICATION_JSON,
    CT_APPLICATION_X_JAVASCRIPT,
    CT_TEXT_CSS,
    CT_TEXT_XML,
    CT_TEXT_XSL,
    CT_APPLICATION_OCTET_STREAM,
    CT_IMAGE_SVG_XML,
    CT_APPLICATION_X_FONT_TRUETYPE,
    CT_APPLICATION_X_FONT_OPENTYPE,
    CT_APPLICATION_FONT_WOFF,
    CT_APPLICATION_FONT_WOFF2,
    CT_APPLICATION_VND_MS_FONTOBJ,
    CT_APPLICATION_PDF,
    CT_IMAGE_PNG,
    CT_IMAGE_AVIF,
    CT_IMAGE_WEBP,
    CT_IMAGE_JPG,
    CT_IMAGE_GIF,
    CT_IMAGE_XICON,
    CT_IMAGE_BMP,
    CT_IMAGE_ICNS,
    CT_APPLICATION_WASM,
    CT_TEXT_PLAIN,
    CT_MULTIPART_FORM_DATA,
    CT_CUSTOM,
    CT_APPLICATION_X_JAVASCRIPT = CT_APPLICATION_X_JAVASCRIPT // to avoid redefinition warning
};

enum FileType
{
    FT_UNKNOWN,
    FT_DOCUMENT,
    FT_ARCHIVE,
    FT_AUDIO,
    FT_MEDIA,
    FT_IMAGE,
    FT_CUSTOM
};

const std::string_view &statusCodeToString(int code);

ContentType getContentType(const std::string &fileName);

ContentType parseContentType(const std::string_view &contentType);

FileType parseFileType(const std::string_view &fileExtension);

const std::string_view &contentTypeToMime(ContentType contenttype);

void registerCustomExtensionMime(const std::string &ext, const std::string &mime);

const std::string_view fileNameToMime(const std::string &fileName);

std::pair<ContentType, const std::string_view> fileNameToContentTypeAndMime(const std::string &fileName);

}  // namespace drogon