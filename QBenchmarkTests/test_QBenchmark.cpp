#include <catch.hpp>

#include <string>
#include <regex>
#include <memory>
#include <iostream>

#include <QBenchmark.h>

using namespace QBenchmark;

class MyTimeProvider : public ITimeProvider
{
public:
    MyTimeProvider() = default;
    std::chrono::microseconds getTimeSinceEpochMsecs() const override { return std::chrono::microseconds(0); }
};

void helperFunctionInternal()
{
    QBenchmarkStart();
    QBenchmarkEnd();
}

void helperFunction()
{
    QBenchmarkStart();
    helperFunctionInternal();
    QBenchmarkEnd();
}

TEST_CASE("Init", "[QBenchmark]")
{
    std::shared_ptr<MyTimeProvider> time = std::make_shared<MyTimeProvider>();
    QBenchmarkRegisterer::getInstance(time);
    std::regex regex("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");

    std::string realOutput;
    realOutput << QBenchmarkRegisterer::getInstance();

    std::vector<std::string> strings;
    std::istringstream f(realOutput);
    std::string s;
    while (getline(f, s, '\n')) {
        REQUIRE(std::regex_match(s, regex) == 1);
    }
}

TEST_CASE("One API call", "[QBenchmark]")
{
    QBenchmarkRegisterer::getInstance();
    std::regex regexFirstLine("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");
    std::regex regexOther("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\} done in \\{[\\d]+.[\\d]+\\} msec.?$");

    QBenchmarkStart();
    QBenchmarkEnd();

    std::string realOutput;
    realOutput << QBenchmarkRegisterer::getInstance();

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

TEST_CASE("3 nested calls", "[QBenchmark]")
{
    QBenchmarkRegisterer::getInstance();
    std::regex regexFirstLine("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\}\\s\\-\\s[\\w\\s]+[\\!]?$");
    std::regex regexOther("^[ ]{0,}\\[[\\d]+\\] \\{[\\w _\\-\\(\\)\\:]+\\} done in \\{[\\d]+.[\\d]+\\} msec.?$");

    QBenchmarkStart();
    helperFunction();
    QBenchmarkEnd();

    std::string realOutput;
    realOutput << QBenchmarkRegisterer::getInstance();

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

