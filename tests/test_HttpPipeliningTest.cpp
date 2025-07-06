#include "gtest/gtest.h"
#include "HttpPipeliningTest.hpp"

TEST(HttpPipeliningTest, RunTest)
{
    EXPECT_NO_THROW(runHttpPipeliningTest());
}