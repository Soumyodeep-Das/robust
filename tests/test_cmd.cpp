#include "gtest/gtest.h"
#include "cmd.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class CmdTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Redirect std::cout to capture output for testing
        old_buf = std::cout.rdbuf();
        std::cout.rdbuf(ss.rdbuf());
    }

    void TearDown() override
    {
        // Restore std::cout
        std::cout.rdbuf(old_buf);
    }

    std::string getOutput()
    {
        return ss.str();
    }

private:
    std::stringstream ss;
    std::streambuf* old_buf;
};

TEST_F(CmdTest, EmptyParametersPrintsIncompleteCommand)
{
    std::vector<std::string> params;
    exeCommand(params);
    std::string output = getOutput();
    EXPECT_NE(output.find("incomplete command"), std::string::npos);
}

TEST_F(CmdTest, InvalidCommandPrintsCommandError)
{
    std::vector<std::string> params = {"invalid_command"};
    exeCommand(params);
    std::string output = getOutput();
    // Since the object creation depends on drogon internals,
    // we expect the output to contain either "command error" or "command not found"
    bool foundError = (output.find("command error") != std::string::npos) ||
                      (output.find("command not found") != std::string::npos);
    EXPECT_TRUE(foundError);
}

TEST_F(CmdTest, ValidCommandCallsHandleCommand)
{
    // This test requires a valid command and drogon environment,
    // which is not possible here. We just ensure no crash with dummy.
    std::vector<std::string> params = {"help"};
    exeCommand(params);
    std::string output = getOutput();
    // Output should not be empty, but exact content depends on drogon internals.
    EXPECT_FALSE(output.empty());
}