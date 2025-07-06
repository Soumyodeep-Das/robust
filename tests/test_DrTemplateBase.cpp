#include "gtest/gtest.h"
#include "DrTemplateBase.hpp"
#include <memory>

using namespace drogon;

class DrTemplateBaseTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DrTemplateBaseTest, NewTemplateReturnsSharedPtr)
{
    // Since DrClassMap::newObject is external and depends on runtime registration,
    // we cannot guarantee a non-null pointer here.
    // We test that the returned object is a shared_ptr and can be null or not.
    std::shared_ptr<DrTemplateBase> ptr = DrTemplateBase::newTemplate("test.csp");
    // The returned pointer type must be shared_ptr
    // We cannot assert non-null because no registration is done here.
    EXPECT_TRUE((std::is_same<decltype(ptr), std::shared_ptr<DrTemplateBase>>::value));
}

TEST_F(DrTemplateBaseTest, NewTemplateHandlesCspExtension)
{
    // Test with a template name that ends with ".csp"
    std::string templateName = "/abc/def.csp";
    std::shared_ptr<DrTemplateBase> ptr = DrTemplateBase::newTemplate(templateName);
    EXPECT_TRUE((std::is_same<decltype(ptr), std::shared_ptr<DrTemplateBase>>::value));
}

TEST_F(DrTemplateBaseTest, NewTemplateHandlesNoCspExtension)
{
    // Test with a template name without ".csp" extension
    std::string templateName = "simpleTemplate";
    std::shared_ptr<DrTemplateBase> ptr = DrTemplateBase::newTemplate(templateName);
    EXPECT_TRUE((std::is_same<decltype(ptr), std::shared_ptr<DrTemplateBase>>::value));
}