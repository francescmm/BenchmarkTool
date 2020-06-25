#include "TimeProvider.h"

using namespace std::chrono;

namespace Benchmarker
{

microseconds Benchmarker::TimeProvider::getTimeSinceEpochMsecs() const
{
   return duration_cast<microseconds>(system_clock::now().time_since_epoch());
}

}
