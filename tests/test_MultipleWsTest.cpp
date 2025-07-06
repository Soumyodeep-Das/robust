#include "MultipleWsTest.hpp"
#include <gtest/gtest.h>
#include <drogon/drogon_test.h>

using namespace drogon;

TEST(MultipleWsTest, BasicFunctionality)
{
    // Since the original test is asynchronous and depends on network,
    // here we just verify that DataPack can be constructed and members are accessible.
    WebSocketClientPtr dummyWsPtr = nullptr;
    std::shared_ptr<test::CaseBase> dummyCtx = nullptr;
    DataPack pack{dummyWsPtr, dummyCtx};

    EXPECT_EQ(pack.wsPtr, nullptr);
    EXPECT_EQ(pack.TEST_CTX, nullptr);
}

// Note: The original DROGON_TEST macro defines its own test that runs the actual test logic.
// We do not replicate the asynchronous network test here because it requires the Drogon framework runtime environment.
// The original test remains in MultipleWsTest.cpp as a DROGON_TEST macro.

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}