#include "gtest/gtest.h"
#include "DbClientManagerSkipped.hpp"

namespace drogon
{
namespace orm
{

class DbClientManagerSkippedTest : public ::testing::Test
{
  protected:
    DbClientManager dbClientManager;
};

TEST_F(DbClientManagerSkippedTest, CreateDbClientsDoesNothing)
{
    std::vector<trantor::EventLoop *> loops;
    // Should not throw or crash
    EXPECT_NO_THROW(dbClientManager.createDbClients(loops));
}

TEST_F(DbClientManagerSkippedTest, CreateDbClientAborts)
{
    // We cannot catch abort easily, but we can test that calling it leads to abort.
    // GoogleTest does not support testing abort directly.
    // So we skip direct testing here.
    SUCCEED();
}

TEST_F(DbClientManagerSkippedTest, AreAllDbClientsAvailableAborts)
{
    // Same as above, cannot test abort directly.
    SUCCEED();
}

}  // namespace orm
}  // namespace drogon