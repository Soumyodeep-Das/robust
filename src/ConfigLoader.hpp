#ifndef CONFIGLOADER_HPP
#define CONFIGLOADER_HPP

#include <string>
#include <json/json.h>

namespace drogon
{
class ConfigLoader
{
  public:
    explicit ConfigLoader(const std::string &configFile);
    explicit ConfigLoader(const Json::Value &data);
    explicit ConfigLoader(Json::Value &&data);
    ~ConfigLoader();

    void load();

  private:
    Json::Value configJsonRoot_;
    std::string configFile_;
};
}  // namespace drogon

#endif  // CONFIGLOADER_HPP