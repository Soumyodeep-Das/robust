#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace drogon
{
class DrObjectBase;

using DrAllocFunc = std::function<DrObjectBase *()>;

class DrClassMap
{
  public:
    static void registerClass(const std::string &className, const DrAllocFunc &func);
    static DrObjectBase *newObject(const std::string &className);
    static const std::shared_ptr<DrObjectBase> &getSingleInstance(const std::string &className);
    static void setSingleInstance(const std::shared_ptr<DrObjectBase> &ins);
    static std::vector<std::string> getAllClassName();

  private:
    static std::unordered_map<std::string, DrAllocFunc> &getMap();
};
}  // namespace drogon