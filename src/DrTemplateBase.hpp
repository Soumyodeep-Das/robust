#ifndef DRTEMPLATEBASE_HPP
#define DRTEMPLATEBASE_HPP

#include <memory>
#include <string>

namespace drogon
{
class DrTemplateBase
{
  public:
    static std::shared_ptr<DrTemplateBase> newTemplate(const std::string &templateName);
};

}  // namespace drogon

#endif  // DRTEMPLATEBASE_HPP