#include "BenchmarkTracker.h"

#include <BenchmarkTool.h>

#include <utility>

namespace GitQlientTools
{
BenchmarkTracker::BenchmarkTracker(BenchmarkTool &benchmarkTool)
   : mBenchmarkTool(benchmarkTool)
{
   mListenerId = mBenchmarkTool.addListener([this](StatisticsData data) { onDataReceived(data); });
}

BenchmarkTracker::~BenchmarkTracker()
{
   mBenchmarkTool.removeListener(mListenerId);
}

void BenchmarkTracker::onDataReceived(StatisticsData data)
{
   std::lock_guard locker(mMutex);

   BenchmarkTrackerData trackerData;
   trackerData.method = data.methodName;

   if (auto iter = mAggregatedData.find(data.methodName); iter != mAggregatedData.end())
      trackerData = iter->second;

   calculateData(data.executionTime, trackerData);

   mAggregatedData[data.methodName] = trackerData;
}

void BenchmarkTracker::calculateData(const double executionTimeInMsecs, BenchmarkTrackerData &trackerData)
{
   if (executionTimeInMsecs <= 10)
      ++trackerData.processTime_0_10_msecs;
   else if (executionTimeInMsecs <= 20)
      ++trackerData.processTime_11_20_msecs;
   else if (executionTimeInMsecs <= 30)
      ++trackerData.processTime_21_30_msecs;
   else if (executionTimeInMsecs <= 40)
      ++trackerData.processTime_31_40_msecs;
   else if (executionTimeInMsecs <= 50)
      ++trackerData.processTime_41_50_msecs;
   else if (executionTimeInMsecs <= 60)
      ++trackerData.processTime_51_60_msecs;
   else if (executionTimeInMsecs <= 70)
      ++trackerData.processTime_61_70_msecs;
   else if (executionTimeInMsecs <= 80)
      ++trackerData.processTime_71_80_msecs;
   else if (executionTimeInMsecs <= 90)
      ++trackerData.processTime_81_90_msecs;
   else if (executionTimeInMsecs <= 100)
      ++trackerData.processTime_91_100_msecs;
   else if (executionTimeInMsecs <= 200)
      ++trackerData.processTime_101_200_msecs;
   else if (executionTimeInMsecs <= 300)
      ++trackerData.processTime_201_300_msecs;
   else if (executionTimeInMsecs <= 400)
      ++trackerData.processTime_301_400_msecs;
   else if (executionTimeInMsecs <= 500)
      ++trackerData.processTime_401_500_msecs;
   else if (executionTimeInMsecs <= 1000)
      ++trackerData.processTime_501_1000_msecs;
   else if (executionTimeInMsecs <= 2000)
      ++trackerData.processTime_1001_2000_msecs;
   else if (executionTimeInMsecs <= 3000)
      ++trackerData.processTime_2001_3000_msecs;
   else if (executionTimeInMsecs <= 4000)
      ++trackerData.processTime_3001_4000_msecs;
   else if (executionTimeInMsecs <= 5000)
      ++trackerData.processTime_4001_5000_msecs;
   else if (executionTimeInMsecs <= 10000)
      ++trackerData.processTime_5001_10000_msecs;
   else if (executionTimeInMsecs > 10001)
      ++trackerData.processTime_10001_more_msecs;
}

}
