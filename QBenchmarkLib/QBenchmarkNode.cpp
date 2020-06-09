#include "QBenchmarkNode.h"

namespace QBenchmark
{

QBenchmarkNode::QBenchmarkNode(const std::string &nodeName, const std::string &threadId,
                               QBenchmarkNode *parent, ITimeProvider* provider) noexcept
   : timeProvider(provider)
   , mNodeName(nodeName)
   , mParent(parent)
   , mThreadId(threadId)
{
   if (parent)
      mLevel = parent->getLevel() + 1;

   mStartTime = timeProvider->getTimeSinceEpochMsecs();
}

bool QBenchmarkNode::operator==(const QBenchmarkNode &node) const
{
   return mNodeName == node.mNodeName && mStartTime == node.mStartTime && mLevel == node.mLevel;
}

bool QBenchmarkNode::operator!=(const QBenchmarkNode &node) const
{
    return !(*this == node);
}

std::string &operator<<(std::string &out, const QBenchmarkNode &node)
{
    for (auto i = 0; i < node.mLevel; ++i)
        out += "  ";

    out.append("[" + node.mThreadId + "] {" + node.mNodeName + "}");

    if (!node.mLocked)
        out.append(" - Method not closed yet!");
    else
    {
        out.append(" done in {" + std::to_string(node.getDuration()) + "} msec.");

        if (!node.mComment.empty())
            out += " - Comments {" + node.mComment + "}";

        if (node.mFlag != QBenchmarkNode::Flag::None)
            out.append(" - Force closed");
    }

    out.append("\n");

    for (const auto &child : node.mChildren)
        out << *(child.get());

    return out;
}

std::ostream &operator<<(std::ostream &out, const QBenchmarkNode &node)
{
   std::string msg;

   msg << node;
   out << msg;

   return out;
}

QBenchmarkNode *QBenchmarkNode::addChild(const std::string &nodeName, const std::string &threadId)
{
   mChildren.push_back(std::make_unique<QBenchmarkNode>(nodeName, threadId, this, this->timeProvider));

   return mChildren.back().get();
}

QBenchmarkNode *QBenchmarkNode::addChild(const std::string &nodeName, const std::string &comment,
                                         const std::string &threadId)
{
   mChildren.push_back(std::make_unique<QBenchmarkNode>(nodeName, threadId, this, this->timeProvider));
   mChildren.back()->addComment(comment);

   return mChildren.back().get();
}

QBenchmarkNode *QBenchmarkNode::getNextOpenChild()
{
   for (const auto &child : mChildren)
   {
      if (!child->isClosed())
         return child.get();
   }

   if (mLocked && mParent)
      return mParent->getNextOpenChild();

   return this;
}

QBenchmarkNode *QBenchmarkNode::getRelativeByName(const std::string &nodeName)
{
   if (mParent)
   {
      for (const auto &child : mParent->mChildren)
      {
         if (!child->isClosed() && *child.get() != *this && child->getNodeName() == nodeName)
            return child.get();
      }

      if (mParent)
          return mParent->getRelativeByName(nodeName);
   }

   return nullptr;
}

void QBenchmarkNode::close(Flag flag)
{
   if (!mLocked)
   {
      mEndTime = timeProvider->getTimeSinceEpochMsecs();
      mLocked = true;
      mFlag = flag;

      for (const auto &child : mChildren)
      {
         if (!child.get()->isClosed())
            child.get()->close(Flag::ForceClosed);
      }
   }
}

double QBenchmarkNode::getDuration() const
{
   return mLocked ? static_cast<double>((mEndTime.count() - mStartTime.count()) / 1000) : -1;
}

}
