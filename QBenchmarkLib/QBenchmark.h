#pragma once

/****************************************************************************************
 ** QBenchmark is a library to register benchmarks of a process.
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

#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <map>
#include <iostream>

#include <QBenchmarkNode.h>
#include "QBenchmarkTimeProvider.h"

#include <sstream>
#include <memory>

namespace QBenchmark
{

class QBenchmarkNode;

class QBenchmarkRegisterer
{
public:
   enum class OutputFormat
   {
      PlainText
   };

   static QBenchmarkRegisterer &getInstance(std::shared_ptr<ITimeProvider> timeProvider = std::make_shared<QBenchmarkTimeProvider>());

   ~QBenchmarkRegisterer();

   void startBenchmark(const std::string &methodName);
   void startBenchmark(const std::string &methodName, const std::string &comment);
   void endBenchmark(const std::string &methodName);

   void setOutputFormat(OutputFormat outputFormat) { mFileFormat = outputFormat; }

   friend std::string &operator<<(std::string &out, const QBenchmarkRegisterer &node);
   friend std::ostream &operator<<(std::ostream &out, const QBenchmarkRegisterer &node);

private:
   QBenchmarkRegisterer(std::shared_ptr<ITimeProvider> timeProvider);

   std::shared_ptr<ITimeProvider> mTimeProvider = nullptr;
   std::mutex mMutex;
   std::unique_ptr<QBenchmarkNode> mRootNode;
   std::map<std::string, QBenchmarkNode *> mActiveNode;
   std::string mThreadId;
   OutputFormat mFileFormat = OutputFormat::PlainText;
};

#ifndef QBenchmarkStart
#   define QBenchmarkStart() QBenchmarkRegisterer::getInstance().startBenchmark(__PRETTY_FUNCTION__)
#   define QBenchmarkStartMsg(msg) QBenchmarkRegisterer::getInstance().startBenchmark(__PRETTY_FUNCTION__, msg)
#endif

#ifndef QBenchmarkEnd
#   define QBenchmarkEnd() QBenchmarkRegisterer::getInstance().endBenchmark(__PRETTY_FUNCTION__)
#endif

}
