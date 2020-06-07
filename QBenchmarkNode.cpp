#include "QBenchmarkNode.h"

using namespace std::chrono;

namespace QBenchmark
{

QBenchmarkNode::QBenchmarkNode(const std::string &nodeName, const std::string &threadId,
                               QBenchmarkNode *parent) noexcept
   : mNodeName(nodeName)
   , mStartTime(duration_cast<microseconds>(system_clock::now().time_since_epoch()))
   , mParent(parent)
   , mThreadId(threadId)
{
   if (parent)
      mLevel = parent->getLevel() + 1;
}

bool QBenchmarkNode::operator==(const QBenchmarkNode &node) const
{
   return mNodeName == node.mNodeName && mStartTime == node.mStartTime && mLevel == node.mLevel;
}

bool QBenchmarkNode::operator!=(const QBenchmarkNode &node) const
{
   return !(*this == node);
}

std::ostream &operator<<(std::ostream &out, const QBenchmarkNode &node)
{
   std::string msg;
   for (auto i = 0; i < node.mLevel; ++i)
      msg += "  ";

   msg.append("[" + node.mThreadId + "] {" + node.mNodeName + "}");

   if (!node.mLocked)
   {
      msg.append(" - Method not closed yet!");

      out << msg;
   }
   else
   {
      const auto totalTime = node.mEndTime.count() - node.mStartTime.count();
      msg.append(" done in {" + std::to_string(static_cast<double>(totalTime) / 1000) + "} msec.");

      if (!node.mComment.empty())
         msg += " - Comments {" + node.mComment + "}";

      if (node.mFlag != QBenchmarkNode::Flag::None)
         msg.append(" - Force closed");

      msg.append("\n");

      out << msg;

      for (const auto &child : node.mChildren)
         out << *(child.get());
   }

   return out;
}

QBenchmarkNode *QBenchmarkNode::addChild(const std::string &nodeName, const std::string &threadId)
{
   mChildren.push_back(std::make_unique<QBenchmarkNode>(nodeName, threadId, this));

   return mChildren.back().get();
}

QBenchmarkNode *QBenchmarkNode::addChild(const std::string &nodeName, const std::string &comment,
                                         const std::string &threadId)
{
   mChildren.push_back(std::make_unique<QBenchmarkNode>(nodeName, threadId, this));
   mChildren.back()->addComment(comment);

   return mChildren.back().get();
}

QBenchmarkNode *QBenchmarkNode::getNextOpenChild()
{
   for (const auto &child : mChildren)
      if (!child->isClosed())
         return child.get();

   if (mLocked && mParent)
      return mParent->getNextOpenChild();

   return this;
}

QBenchmarkNode *QBenchmarkNode::searchParent(const std::string &nodeName)
{
   if (mParent)
   {
      for (const auto &child : mParent->mChildren)
      {
         if (!child->isClosed() && *child.get() != *this && child->getNodeName() == nodeName)
            return child.get();
      }

      QBenchmarkNode *node = nullptr;

      for (const auto &child : mParent->mChildren)
      {
         if (child.get() != this && !child->isClosed())
         {
            node = child->searchParent(nodeName);

            if (node)
               return node;
         }
      }
   }

   return nullptr;
}

void QBenchmarkNode::close(Flag flag)
{
   if (!mLocked)
   {
      mEndTime = duration_cast<microseconds>(system_clock::now().time_since_epoch());
      mLocked = true;
      mFlag = flag;

      for (const auto &child : mChildren)
         if (!child.get()->isClosed())
            child.get()->close(Flag::ForceClosed);
   }
}

}
