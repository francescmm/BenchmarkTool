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

#include <memory>
#include <string>
#include <vector>

#include <TimeProvider.h>

namespace GitQlientTools
{

class Node
{
public:
   struct MinimalData
   {
      std::string mNodeName;
      double duration;
   };
   enum class Flag
   {
      None,
      ForceClosed
   };
   explicit Node(const std::string &nodeName, const std::string &threadId, Node *parent,
                 ITimeProvider *provider) noexcept;

   ~Node() = default;

   bool operator==(const Node &node) const;
   bool operator!=(const Node &node) const;

   friend std::ostream &operator<<(std::ostream &out, const Node &node);
   friend std::string &operator<<(std::string &out, const Node &node);

   Node *addChild(const std::string &nodeName, const std::string &comment, const std::string &threadId);
   Node *addChild(const std::string &nodeName, const std::string &comment);
   Node *getNextOpenChild();
   Node *getRelativeByName(const std::string &nodeName);
   Node *getParent() const { return mParent; }
   std::vector<MinimalData> getForceClosedChildren() const;

   void close(Flag flag = Flag::None);
   bool isClosed() { return mLocked; }
   bool isForceClosed() { return mLocked && mFlag == Flag::ForceClosed; }

   std::string getNodeName() const { return mNodeName; }
   int getLevel() const { return mLevel; }

   double getDuration() const;

   void addComment(const std::string &comment) { mComment = comment; }

   MinimalData getMinimalData() { return MinimalData { mNodeName, getDuration() }; }

private:
   ITimeProvider *timeProvider = nullptr;

   std::string mNodeName;
   std::chrono::microseconds mStartTime;
   std::chrono::microseconds mEndTime;
   bool mLocked = false;
   Node *mParent = nullptr;
   std::vector<std::unique_ptr<Node>> mChildren;
   int mLevel = 0;
   std::string mComment;
   Flag mFlag = Flag::None;
   std::string mThreadId;
};

}
