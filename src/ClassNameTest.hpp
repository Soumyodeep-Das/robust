#ifndef CLASSNAMETEST_HPP
#define CLASSNAMETEST_HPP

#include <string>

namespace api
{
namespace v1
{
template <typename T>
class handler
{
  public:
    static std::string name();
    static std::string classTypeName();
    std::string className() const;
};

class hh : public handler<hh>
{
};

}  // namespace v1
}  // namespace api

#endif // CLASSNAMETEST_HPP