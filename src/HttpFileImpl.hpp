#ifndef HTTPFILEIMPL_HPP
#define HTTPFILEIMPL_HPP

#include <string>
#include <memory>
#include <string_view>
#include <filesystem>

namespace drogon
{
class HttpFileImpl
{
  public:
    int save() const;
    int save(const std::string &path) const;
    int saveAs(const std::string &fileName) const;
    int saveTo(const std::filesystem::path &pathAndFileName) const;
    std::string getMd5() const;

    const std::string &getFileName() const;
    void setFileName(const std::string &fileName);
    std::string_view getFileExtension() const;
    enum class FileType
    {
        Unknown,
        Text,
        Binary,
        Image,
        Audio,
        Video,
        Archive
    };
    FileType getFileType() const;
    void setFile(const char *data, size_t length);
    size_t fileLength() const noexcept;
    const char *fileData() const noexcept;
    const std::string &getItemName() const;

  private:
    std::string fileName_;
    std::string fileContent_;
    std::string itemName_;
};

class HttpFile
{
  public:
    explicit HttpFile(std::shared_ptr<HttpFileImpl> &&implPtr);

    const std::string &getFileName() const;
    void setFileName(const std::string &fileName);
    std::string_view getFileExtension() const;
    HttpFileImpl::FileType getFileType() const;
    void setFile(const char *data, size_t length);
    int save() const;
    int save(const std::string &path) const;
    int saveAs(const std::string &fileName) const;
    size_t fileLength() const noexcept;
    const char *fileData() const noexcept;
    std::string getMd5() const;
    const std::string &getItemName() const;

  private:
    std::shared_ptr<HttpFileImpl> implPtr_;
};

}  // namespace drogon

#endif  // HTTPFILEIMPL_HPP