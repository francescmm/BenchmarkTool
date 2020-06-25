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

#include <memory>
#include <string>
#include <vector>
#include <ostream>

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
