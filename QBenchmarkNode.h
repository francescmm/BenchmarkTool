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
#include <vector>
#include <chrono>

namespace QBenchmark
{

class QBenchmarkNode
{
public:
   enum class Flag
   {
      None,
      ForceClosed
   };
   explicit QBenchmarkNode(const std::string &nodeName, const std::string &threadId, QBenchmarkNode *parent) noexcept;
   ~QBenchmarkNode() = default;

   bool operator==(const QBenchmarkNode &node) const;
   bool operator!=(const QBenchmarkNode &node) const;

   friend std::ostream &operator<<(std::ostream &out, const QBenchmarkNode &node);

   QBenchmarkNode *addChild(const std::string &nodeName, const std::string &comment, const std::string &threadId);
   QBenchmarkNode *addChild(const std::string &nodeName, const std::string &comment);
   QBenchmarkNode *getNextOpenChild();
   QBenchmarkNode *searchParent(const std::string &nodeName);
   QBenchmarkNode *getParent() const { return mParent; }

   void close(Flag flag = Flag::None);
   bool isClosed() { return mLocked; }

   std::string getNodeName() const { return mNodeName; }
   int getLevel() const { return mLevel; }

   void addComment(const std::string &comment) { mComment = comment; }

private:
   std::string mNodeName;
   std::chrono::microseconds mStartTime;
   std::chrono::microseconds mEndTime;
   bool mLocked = false;
   QBenchmarkNode *mParent = nullptr;
   std::vector<std::unique_ptr<QBenchmarkNode>> mChildren;
   int mLevel = 0;
   std::string mComment;
   Flag mFlag = Flag::None;
   std::string mThreadId;
};

}
