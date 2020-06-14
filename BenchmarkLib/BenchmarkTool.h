#pragma once

/****************************************************************************************
 ** BenchmarkTool is a library to register benchmarks of a process.
 **
 ** LinkedIn: www.linkedin.com/in/cescmm/
 ** Web: www.francescmm.com
 **
 ** This lbirary is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <TimeProvider.h>
#include <BenchmarkCallback.h>

#include <mutex>
#include <thread>
#include <map>
#include <sstream>
#include <atomic>

namespace GitQlientTools
{

class Node;

class BenchmarkTool
{
public:
   enum class OutputFormat
   {
      PlainText
   };

   static BenchmarkTool &getInstance(std::shared_ptr<ITimeProvider> timeProvider = std::make_shared<TimeProvider>());

   ~BenchmarkTool();

   void startBenchmark(const std::string &methodName);
   void startBenchmark(const std::string &methodName, const std::string &comment);
   void endBenchmark(const std::string &methodName);

   void setOutputFormat(OutputFormat outputFormat) { mFileFormat = outputFormat; }

   int addListener(ListenerCallback callback);
   void removeListener(int listenerId);

   friend std::string &operator<<(std::string &out, const BenchmarkTool &node);
   friend std::ostream &operator<<(std::ostream &out, const BenchmarkTool &node);

private:
   BenchmarkTool(std::shared_ptr<ITimeProvider> timeProvider);

   std::shared_ptr<ITimeProvider> mTimeProvider = nullptr;
   std::mutex mMutex;
   std::unique_ptr<Node> mRootNode;
   std::map<std::string, Node *> mActiveNode;
   std::string mThreadId;
   OutputFormat mFileFormat = OutputFormat::PlainText;
   std::mutex mListenersMutex;
   std::atomic<int> mListenerId { 0 };
   std::map<int, ListenerCallback> mListeners;
};

#ifndef PLATFORM_WINDOWS
#   ifndef BenchmarkStart
#      define BenchmarkStart() BenchmarkTool::getInstance().startBenchmark(__PRETTY_FUNCTION__)
#      define BenchmarkStartMsg(msg) BenchmarkTool::getInstance().startBenchmark(__PRETTY_FUNCTION__, msg)
#   endif
#else
#   ifndef BenchmarkStart
#      define BenchmarkStart() BenchmarkTool::getInstance().startBenchmark(__FUNCSIG__)
#      define BenchmarkStartMsg(msg) BenchmarkTool::getInstance().startBenchmark(__FUNCSIG__, msg)
#   endif
#endif

#ifndef PLATFORM_WINDOWS
#   ifndef BenchmarkEnd
#      define BenchmarkEnd() BenchmarkTool::getInstance().endBenchmark(__PRETTY_FUNCTION__)
#   endif
#else
#   ifndef BenchmarkEnd
#      define BenchmarkEnd() BenchmarkTool::getInstance().endBenchmark(__FUNCSIG__)
#   endif
#endif

}
