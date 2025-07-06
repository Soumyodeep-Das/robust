#include "ClassNameTest.hpp"

namespace api
{
namespace v1
{

template <typename T>
std::string handler<T>::name()
{
    return handler<T>::classTypeName();
}

template <typename T>
std::string handler<T>::classTypeName()
{
    return typeid(T).name();
}

template <typename T>
std::string handler<T>::className() const
{
    return handler<T>::classTypeName();
}

// Explicit template instantiation for hh
template class handler<hh>;

}  // namespace v1
}  // namespace api