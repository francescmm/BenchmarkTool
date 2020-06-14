#include "Node.h"

namespace GitQlientTools
{

Node::Node(const std::string &nodeName, const std::string &threadId, Node *parent, ITimeProvider *provider) noexcept
   : timeProvider(provider)
   , mNodeName(nodeName)
   , mParent(parent)
   , mThreadId(threadId)
{
   if (parent)
      mLevel = parent->getLevel() + 1;

   mStartTime = timeProvider->getTimeSinceEpochMsecs();
}

bool Node::operator==(const Node &node) const
{
   return mNodeName == node.mNodeName && mStartTime == node.mStartTime && mLevel == node.mLevel;
}

bool Node::operator!=(const Node &node) const
{
   return !(*this == node);
}

std::string &operator<<(std::string &out, const Node &node)
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

      if (node.mFlag != Node::Flag::None)
         out.append(" - Force closed");
   }

   out.append("\n");

   for (const auto &child : node.mChildren)
      out << *(child.get());

   return out;
}

std::ostream &operator<<(std::ostream &out, const Node &node)
{
   std::string msg;

   msg << node;
   out << msg;

   return out;
}

Node *Node::addChild(const std::string &nodeName, const std::string &threadId)
{
   mChildren.push_back(std::make_unique<Node>(nodeName, threadId, this, this->timeProvider));

   return mChildren.back().get();
}

Node *Node::addChild(const std::string &nodeName, const std::string &comment, const std::string &threadId)
{
   mChildren.push_back(std::make_unique<Node>(nodeName, threadId, this, this->timeProvider));
   mChildren.back()->addComment(comment);

   return mChildren.back().get();
}

Node *Node::getNextOpenChild()
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

Node *Node::getRelativeByName(const std::string &nodeName)
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

std::vector<Node::MinimalData> Node::getForceClosedChildren() const
{
   std::vector<MinimalData> forceClosed;

   for (const auto &child : mChildren)
   {
      if (child->isForceClosed())
      {
         forceClosed.emplace_back(child->getMinimalData());

         const std::vector<MinimalData> other = child->getForceClosedChildren();
         forceClosed.insert(forceClosed.end(), other.begin(), other.end());
      }
   }

   return forceClosed;
}

void Node::close(Flag flag)
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

double Node::getDuration() const
{
   return mLocked ? static_cast<double>((mEndTime.count() - mStartTime.count()) / 1000) : -1;
}
}
