#include "gtest/gtest.h"
#include "JwtPlugin.hpp"
#include <drogon/drogon.h>

TEST(JwtPluginTest, TestInitAndStartAndInit) {
    JwtPlugin plugin;
    drogon::Json::Value config;
    config["secret"] = "test_secret";
    config["sessionTime"] = 3600; // int value
    config["issuer"] = "test_issuer";
    plugin.initAndStart(config);
    EXPECT_NO_THROW({
        auto jwt = plugin.init(); // ensure init can be called without throwing
        (void)jwt;
    });
}

TEST(JwtPluginTest, TestShutdown) {
    JwtPlugin plugin;
    EXPECT_NO_THROW(plugin.shutdown()); // ensure shutdown can be called without throwing
}