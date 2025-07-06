#include "gtest/gtest.h"
#include "ossp_uuid_lib_test.hpp"

TEST(UUIDLibTest, CreateUUID)
{
    EXPECT_NO_THROW(uuid_test_create());
}

TEST(UUIDLibTest, MakeUUID)
{
    EXPECT_NO_THROW(uuid_test_make());
}