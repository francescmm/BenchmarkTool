#include <catch.hpp>

#include <BenchmarkTool.h>
#include <BenchmarkTracker.h>

using namespace Benchmarker;

void trackerHelperFunctionInternal()
{
   BenchmarkStart();
   BenchmarkEnd();
}

void trackerHelperFunction()
{
   BenchmarkStart();
   trackerHelperFunctionInternal();
   BenchmarkEnd();
}

auto &instance = BenchmarkTool::getInstance();

TEST_CASE("Init tracker", "[BenchmarkTracker]")
{
   BenchmarkTracker tracker(instance);
   const auto data = tracker.getAggregatedData();

   REQUIRE(data.empty());
}

TEST_CASE("Tracking one API call", "[BenchmarkTracker]")
{
   BenchmarkTracker tracker(instance);

   BenchmarkStart();
   BenchmarkEnd();

   const auto testMethod = "void ____C_A_T_C_H____T_E_S_T____2()";
   const auto data = tracker.getAggregatedData();
   const auto iter = data.find(testMethod);
   const auto found = iter != data.end();

   REQUIRE(found);
   REQUIRE(iter->second.method == testMethod);
}

TEST_CASE("Tracking 3 nested calls", "[BenchmarkTracker]")
{
   BenchmarkTracker tracker(instance);

   BenchmarkStart();
   trackerHelperFunction();
   BenchmarkEnd();

   const auto testMethod = "void trackerHelperFunction()";
   const auto data = tracker.getAggregatedData();
   const auto iter = data.find(testMethod);
   const auto found = iter != data.end();

   REQUIRE(found);
   REQUIRE(iter->second.method == testMethod);
}
