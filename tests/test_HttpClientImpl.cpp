#include "gtest/gtest.h"
#include "HttpClientImpl.hpp"

using namespace drogon;

class HttpClientImplTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HttpClientImplTest, ConstructorWithInetAddress)
{
    trantor::EventLoop loop;
    trantor::InetAddress addr("127.0.0.1", 80);
    HttpClientImpl client(&loop, addr, false, false, true);
    EXPECT_EQ(client.port(), 80);
    EXPECT_EQ(client.host(), "127.0.0.1");
}

TEST_F(HttpClientImplTest, ConstructorWithHostStringHttp)
{
    trantor::EventLoop loop;
    HttpClientImpl client(&loop, "http://example.com", false, true);
    EXPECT_EQ(client.host(), "example.com");
    EXPECT_EQ(client.port(), 80);
}

TEST_F(HttpClientImplTest, ConstructorWithHostStringHttps)
{
    trantor::EventLoop loop;
    HttpClientImpl client(&loop, "https://example.com", false, true);
    EXPECT_EQ(client.host(), "example.com");
    EXPECT_EQ(client.port(), 443);
}

TEST_F(HttpClientImplTest, SetCertPathAndAddSSLConfigs)
{
    trantor::EventLoop loop;
    trantor::InetAddress addr("127.0.0.1", 443);
    HttpClientImpl client(&loop, addr, true, false, true);
    client.setCertPath("cert.pem", "key.pem");

    std::vector<std::pair<std::string, std::string>> sslConfigs = {
        {"SSLProtocol", "TLSv1.2"},
        {"CipherString", "HIGH:!aNULL:!MD5"}};
    client.addSSLConfigs(sslConfigs);
    // No direct getter, so just ensure no crash and internal state updated
    SUCCEED();
}

TEST_F(HttpClientImplTest, OnDefaultPort)
{
    trantor::EventLoop loop;
    trantor::InetAddress addr1("127.0.0.1", 80);
    HttpClientImpl client1(&loop, addr1, false, false, true);
    EXPECT_TRUE(client1.onDefaultPort());

    trantor::InetAddress addr2("127.0.0.1", 8080);
    HttpClientImpl client2(&loop, addr2, false, false, true);
    EXPECT_FALSE(client2.onDefaultPort());

    trantor::InetAddress addr3("127.0.0.1", 443);
    HttpClientImpl client3(&loop, addr3, true, false, true);
    EXPECT_TRUE(client3.onDefaultPort());
}

TEST_F(HttpClientImplTest, HostAndPort)
{
    trantor::EventLoop loop;
    trantor::InetAddress addr("example.com", 1234);
    HttpClientImpl client(&loop, addr, false, false, true);
    EXPECT_EQ(client.host(), "example.com");
    EXPECT_EQ(client.port(), 1234);
}