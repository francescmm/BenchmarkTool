#pragma once

/****************************************************************************
**
** Copyright (C) 2020 Francesc Martinez
** LinkedIn: www.linkedin.com/in/cescmm/
** Web: www.francescmm.com
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of Francesc M. nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#include <BenchmarkCallback.h>

#include <map>
#include <mutex>

namespace GitQlientTools
{

class BenchmarkTool;

struct BenchmarkTrackerData
{
   std::string method;
   unsigned int totalSuccess = 0;
   unsigned int totalForced = 0;
   unsigned int processTime_0_10_msecs = 0;
   unsigned int processTime_11_20_msecs = 0;
   unsigned int processTime_21_30_msecs = 0;
   unsigned int processTime_31_40_msecs = 0;
   unsigned int processTime_41_50_msecs = 0;
   unsigned int processTime_51_60_msecs = 0;
   unsigned int processTime_61_70_msecs = 0;
   unsigned int processTime_71_80_msecs = 0;
   unsigned int processTime_81_90_msecs = 0;
   unsigned int processTime_91_100_msecs = 0;
   unsigned int processTime_101_200_msecs = 0;
   unsigned int processTime_201_300_msecs = 0;
   unsigned int processTime_301_400_msecs = 0;
   unsigned int processTime_401_500_msecs = 0;
   unsigned int processTime_501_1000_msecs = 0;
   unsigned int processTime_1001_2000_msecs = 0;
   unsigned int processTime_2001_3000_msecs = 0;
   unsigned int processTime_3001_4000_msecs = 0;
   unsigned int processTime_4001_5000_msecs = 0;
   unsigned int processTime_5001_10000_msecs = 0;
   unsigned int processTime_10001_more_msecs = 0;
};

class BenchmarkTracker
{
public:
   BenchmarkTracker(BenchmarkTool &benchmarkTool);
   ~BenchmarkTracker();

   std::map<std::string, BenchmarkTrackerData> getAggregatedData() const { return mAggregatedData; }

private:
   BenchmarkTool &mBenchmarkTool;
   std::map<std::string, BenchmarkTrackerData> mAggregatedData;
   std::mutex mMutex;
   int mListenerId = -1;

   void onDataReceived(StatisticsData data);
   void calculateData(double executionTimeInMsecs, BenchmarkTrackerData &trackerData);
};

}
