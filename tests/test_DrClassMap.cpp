#include "DrClassMap.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace drogon;

class DummyObject : public DrObjectBase
{
  public:
    DummyObject() = default;
    virtual ~DummyObject() = default;
    std::string className() const override { return "DummyObject"; }
};

DrObjectBase *dummyAllocFunc()
{
    return new DummyObject();
}

TEST(DrClassMapTest, RegisterAndCreateObject)
{
    DrClassMap::registerClass("DummyObject", dummyAllocFunc);
    DrObjectBase *obj = DrClassMap::newObject("DummyObject");
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->className(), "DummyObject");
    delete obj;
}

TEST(DrClassMapTest, NewObjectReturnsNullForUnknownClass)
{
    DrObjectBase *obj = DrClassMap::newObject("UnknownClass");
    EXPECT_EQ(obj, nullptr);
}

TEST(DrClassMapTest, SingleInstanceManagement)
{
    DrClassMap::registerClass("DummyObject", dummyAllocFunc);
    const auto &instance1 = DrClassMap::getSingleInstance("DummyObject");
    ASSERT_NE(instance1.get(), nullptr);
    EXPECT_EQ(instance1->className(), "DummyObject");

    auto newInstance = std::make_shared<DummyObject>();
    DrClassMap::setSingleInstance(newInstance);
    const auto &instance2 = DrClassMap::getSingleInstance("DummyObject");
    EXPECT_EQ(instance2, newInstance);
}

TEST(DrClassMapTest, GetAllClassNameContainsRegisteredClasses)
{
    DrClassMap::registerClass("DummyObject", dummyAllocFunc);
    auto classNames = DrClassMap::getAllClassName();
    auto found = std::find(classNames.begin(), classNames.end(), "DummyObject");
    EXPECT_NE(found, classNames.end());
}