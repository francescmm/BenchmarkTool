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

      for (const auto &listenerIter : mListeners)
      {
         listenerIter.second(StatisticsData { iter->second->getNodeName(), iter->second->getDuration() });

         for (auto &child : iter->second->getForceClosedChildren())
            listenerIter.second(StatisticsData { child.mNodeName, child.duration });
      }

      mActiveNode[threadId] = iter->second->getNextOpenChild();
   }
   else if (auto node = iter->second->getRelativeByName(methodName))
   {
      node->close();

      for (const auto &listenerIter : mListeners)
      {
         listenerIter.second(StatisticsData { node->getNodeName(), node->getDuration() });

         for (auto &child : node->getForceClosedChildren())
            listenerIter.second(StatisticsData { child.mNodeName, child.duration });
      }

      mActiveNode[threadId] = node->getParent();
   }
}

int BenchmarkTool::addListener(ListenerCallback callback)
{
   std::lock_guard<std::mutex> lock(mListenersMutex);

   const auto id = ++mListenerId;

   mListeners.emplace(id, std::move(callback));
   return id;
}

void BenchmarkTool::removeListener(int listenerId)
{
   std::lock_guard<std::mutex> lock(mListenersMutex);

   if (auto iter = mListeners.find(listenerId); iter != mListeners.end())
      mListeners.erase(iter);
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
