#include "QBenchmarkTimeProvider.h"

using namespace std::chrono;

namespace QBenchmark {

microseconds QBenchmark::QBenchmarkTimeProvider::getTimeSinceEpochMsecs() const
{
    return duration_cast<microseconds>(system_clock::now().time_since_epoch());
}

}
