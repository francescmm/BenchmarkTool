#include <catch.hpp>

#include <string>
#include <regex>
#include <memory>
#include <iostream>

#include <BenchmarkTool.h>

using namespace GitQlientTools;

class MyTimeProvider : public ITimeProvider
{
public:
    MyTimeProvider() = default;
    std::chrono::microseconds getTimeSinceEpochMsecs() const override { return std::chrono::microseconds(0); }
};

void helperFunctionInternal()
{
    BenchmarkStart();
    BenchmarkEnd();
}

void helperFunction()
{
    BenchmarkStart();
    helperFunctionInternal();
    BenchmarkEnd();
}

TEST_CASE("Init", "[BenchmarkTool]")
{
    std::shared_ptr<MyTimeProvider> time = std::make_shared<MyTimeProvider>();
    BenchmarkTool::getInstance(time);
    std::regex regex("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");

    std::string realOutput;
    realOutput << BenchmarkTool::getInstance();

    std::vector<std::string> strings;
    std::istringstream f(realOutput);
    std::string s;
    while (getline(f, s, '\n')) {
        REQUIRE(std::regex_match(s, regex) == 1);
    }
}

TEST_CASE("One API call", "[BenchmarkTool]")
{
    BenchmarkTool::getInstance();
    std::regex regexFirstLine("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");
    std::regex regexOther("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\} done in \\{[\\d]+.[\\d]+\\} msec.?$");

    BenchmarkStart();
    BenchmarkEnd();

    std::string realOutput;
    realOutput << BenchmarkTool::getInstance();

    std::vector<std::string> strings;
    std::istringstream f(realOutput);
    std::string s;
    auto i = 0;

    for (; getline(f, s, '\n'); ++i) {
        if (i == 0)
            REQUIRE(std::regex_match(s, regexFirstLine) == 1);
        else
            REQUIRE(std::regex_match(s, regexOther) == 1);
    }

    // How many calls are we waiting. 1-index based to improve readability
    REQUIRE(i == 2);
}

TEST_CASE("3 nested calls", "[BenchmarkTool]")
{
    BenchmarkTool::getInstance();
    std::regex regexFirstLine("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");
    std::regex regexOther("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\} done in \\{[\\d]+.[\\d]+\\} msec.?$");

    BenchmarkStart();
    helperFunction();
    BenchmarkEnd();

    std::string realOutput;
    realOutput << BenchmarkTool::getInstance();

    std::vector<std::string> strings;
    std::istringstream f(realOutput);
    std::string s;
    auto i = 0;

    for (; getline(f, s, '\n'); ++i) {
        if (i == 0)
            REQUIRE(std::regex_match(s, regexFirstLine) == 1);
        else
            REQUIRE(std::regex_match(s, regexOther) == 1);
    }

    // How many calls are we waiting. 1-index based to improve readability
    REQUIRE(i == 5);
}

