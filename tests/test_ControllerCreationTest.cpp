#include "gtest/gtest.h"
#include "ControllerCreationTest.hpp"

TEST(ControllerCreation, InitialCreatedFlags)
{
    EXPECT_FALSE(Ctrl::created);
    EXPECT_FALSE(SimpleCtrl::created);
    EXPECT_FALSE(WsCtrl::created);
}