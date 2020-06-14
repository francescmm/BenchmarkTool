#pragma once

#include <functional>
#include <chrono>
#include <string>

namespace GitQlientTools
{

struct StatisticsData
{
   std::string methodName;
   double executionTime;
};

using ListenerCallback = std::function<void(StatisticsData)>;

}
