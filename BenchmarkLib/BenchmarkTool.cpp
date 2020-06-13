#include "BenchmarkTool.h"

#include "Node.h"

#include <fstream>

namespace GitQlientTools
{

BenchmarkTool &BenchmarkTool::getInstance(std::shared_ptr<ITimeProvider> timeProvider)
{
   static BenchmarkTool instance(timeProvider);

   return instance;
}

BenchmarkTool::BenchmarkTool(std::shared_ptr<ITimeProvider> timeProvider)
   : mTimeProvider(timeProvider)
{
   std::stringstream ss;
   ss << std::this_thread::get_id();

   mThreadId = ss.str();
   mRootNode = std::make_unique<Node>("BenchmarkTool", mThreadId, nullptr, mTimeProvider.get());
   mActiveNode[mThreadId] = mRootNode.get();
}

BenchmarkTool::~BenchmarkTool()
{
   mRootNode->close();

   std::ofstream myfile;
   myfile.open("logs/BenchmarkTool.txt", std::ios::out | std::ios::app);
   myfile << (*mRootNode.get());
   myfile.flush();
   myfile.close();
}

void BenchmarkTool::startBenchmark(const std::string &methodName)
{
   startBenchmark(methodName, "");
}

void BenchmarkTool::startBenchmark(const std::string &methodName, const std::string &comment)
{
   std::lock_guard<std::mutex> lock { mMutex };

   std::stringstream ss;
   ss << std::this_thread::get_id();

   const auto threadId = ss.str();

   if (auto iter = mActiveNode.find(threadId); iter != mActiveNode.end() && !iter->second->isClosed())
      mActiveNode[threadId] = iter->second->addChild(methodName, comment, threadId);
   else
      mActiveNode[threadId] = mRootNode->addChild(methodName, comment, threadId);
}

void BenchmarkTool::endBenchmark(const std::string &methodName)
{
   std::lock_guard<std::mutex> lock { mMutex };

   std::stringstream ss;
   ss << std::this_thread::get_id();
   const auto threadId = ss.str();
   const auto iter = mActiveNode.find(threadId);

   if (iter == mActiveNode.end())
      return;

   if (iter->second->getNodeName() == methodName)
   {
      iter->second->close();
      mActiveNode[threadId] = iter->second->getNextOpenChild();
   }
   else if (auto node = iter->second->getRelativeByName(methodName))
   {
      node->close();
      mActiveNode[threadId] = node->getParent();
   }
}

std::string &operator<<(std::string &out, const BenchmarkTool &node)
{
   out << (*node.mRootNode.get());

   return out;
}

std::ostream &operator<<(std::ostream &out, const BenchmarkTool &node)
{
   out << (*node.mRootNode.get());

   return out;
}

}
