#include "gtest/gtest.h"
#include "create_controller.h"
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace drogon_ctl;

class CreateControllerTest : public ::testing::Test
{
protected:
    create_controller controller;

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(CreateControllerTest, NewSimpleControllerHeaderFileBasic)
{
    std::ostringstream oss;
    std::string className = "TestSimple";
    std::ofstream ofs("test_simple.h");
    controller.newSimpleControllerHeaderFile(ofs, className);
    ofs.close();

    std::ifstream ifs("test_simple.h");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("class TestSimple"), std::string::npos);
    EXPECT_NE(content.find("asyncHandleHttpRequest"), std::string::npos);

    std::remove("test_simple.h");
}

TEST_F(CreateControllerTest, NewSimpleControllerSourceFileBasic)
{
    std::string className = "TestSimple";
    std::string filename = "test_simple";
    std::ofstream ofs(filename + ".cc");
    controller.newSimpleControllerSourceFile(ofs, className, filename);
    ofs.close();

    std::ifstream ifs(filename + ".cc");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("void TestSimple::asyncHandleHttpRequest"), std::string::npos);

    std::remove((filename + ".cc").c_str());
}

TEST_F(CreateControllerTest, NewWebsockControllerHeaderFileBasic)
{
    std::string className = "TestWebSocket";
    std::ofstream ofs("test_websocket.h");
    controller.newWebsockControllerHeaderFile(ofs, className);
    ofs.close();

    std::ifstream ifs("test_websocket.h");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("class TestWebSocket"), std::string::npos);
    EXPECT_NE(content.find("handleNewMessage"), std::string::npos);

    std::remove("test_websocket.h");
}

TEST_F(CreateControllerTest, NewWebsockControllerSourceFileBasic)
{
    std::string className = "TestWebSocket";
    std::string filename = "test_websocket";
    std::ofstream ofs(filename + ".cc");
    controller.newWebsockControllerSourceFile(ofs, className, filename);
    ofs.close();

    std::ifstream ifs(filename + ".cc");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("void TestWebSocket::handleNewMessage"), std::string::npos);
    EXPECT_NE(content.find("void TestWebSocket::handleNewConnection"), std::string::npos);
    EXPECT_NE(content.find("void TestWebSocket::handleConnectionClosed"), std::string::npos);

    std::remove((filename + ".cc").c_str());
}

TEST_F(CreateControllerTest, NewHttpControllerHeaderFileBasic)
{
    std::string className = "TestHttp";
    std::ofstream ofs("test_http.h");
    controller.newHttpControllerHeaderFile(ofs, className);
    ofs.close();

    std::ifstream ifs("test_http.h");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("class TestHttp"), std::string::npos);
    EXPECT_NE(content.find("METHOD_LIST_BEGIN"), std::string::npos);

    std::remove("test_http.h");
}

TEST_F(CreateControllerTest, NewHttpControllerSourceFileBasic)
{
    std::string className = "TestHttp";
    std::string filename = "test_http";
    std::ofstream ofs(filename + ".cc");
    controller.newHttpControllerSourceFile(ofs, className, filename);
    ofs.close();

    std::ifstream ifs(filename + ".cc");
    ASSERT_TRUE(ifs.is_open());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();

    EXPECT_NE(content.find("// Add definition of your processing function here"), std::string::npos);

    std::remove((filename + ".cc").c_str());
}

TEST_F(CreateControllerTest, CreateControllerVectorSimple)
{
    std::vector<std::string> classes = {"TestSimple1", "TestSimple2"};
    controller.createController(classes, Simple);
    // This test mainly checks no exceptions and no output errors.
    SUCCEED();
}

TEST_F(CreateControllerTest, CreateControllerSingleSimple)
{
    controller.createController("TestSimpleSingle", Simple);
    SUCCEED();
}

TEST_F(CreateControllerTest, CreateARestfulControllerBasic)
{
    // We cannot test file creation easily without mocking templates,
    // so just test that it runs without throwing.
    ASSERT_NO_THROW(controller.createARestfulController("TestRestful", "resource"));
}

TEST_F(CreateControllerTest, HandleCommandSimple)
{
    std::vector<std::string> params = {"-s", "MyClass"};
    controller.handleCommand(params);
    SUCCEED();
}

TEST_F(CreateControllerTest, HandleCommandHttp)
{
    std::vector<std::string> params = {"--http", "MyClass"};
    controller.handleCommand(params);
    SUCCEED();
}

TEST_F(CreateControllerTest, HandleCommandWebSocket)
{
    std::vector<std::string> params = {"--websocket", "MyClass"};
    controller.handleCommand(params);
    SUCCEED();
}

TEST_F(CreateControllerTest, HandleCommandRestful)
{
    std::vector<std::string> params = {"--restful", "MyClass"};
    controller.handleCommand(params);
    SUCCEED();
}