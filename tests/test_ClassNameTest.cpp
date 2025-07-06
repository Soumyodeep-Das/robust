#include "gtest/gtest.h"
#include "ClassNameTest.hpp"

namespace api
{
namespace v1
{

TEST(ClassNameTest, HhClassName)
{
    hh h;
    std::string expected = typeid(hh).name();

    // Because typeid(T).name() returns implementation defined string,
    // we test consistency between calls.
    EXPECT_EQ(h.className(), expected);
    EXPECT_EQ(hh::classTypeName(), expected);
    EXPECT_EQ(h.name(), expected);
}

}  // namespace v1
}  // namespace api