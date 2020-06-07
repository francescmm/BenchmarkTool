#include "QBenchmark.h"

#include "QBenchmarkNode.h"

#include <fstream>

namespace QBenchmark
{

QBenchmarkRegisterer &QBenchmarkRegisterer::getInstance()
{
   static QBenchmarkRegisterer instance;

   return instance;
}

QBenchmarkRegisterer::QBenchmarkRegisterer()
{
   std::stringstream ss;
   ss << std::this_thread::get_id();

   mThreadId = ss.str();
   mRootNode = std::make_unique<QBenchmarkNode>("QBenchmarkRegisterer", mThreadId, nullptr);
   mActiveNode[mThreadId] = mRootNode.get();
}

QBenchmarkRegisterer::~QBenchmarkRegisterer()
{
   mRootNode->close();

   std::ofstream myfile;
   myfile.open("logs/QBenchmark.txt", std::ios::out | std::ios::app);
   myfile << (*mRootNode.get());
   myfile.flush();
   myfile.close();
}

void QBenchmarkRegisterer::startBenchmark(const std::string &methodName)
{
   std::lock_guard<std::mutex> lock { mMutex };

   std::stringstream ss;
   ss << std::this_thread::get_id();

   const auto threadId = ss.str();

   if (auto iter = mActiveNode.find(threadId); iter != mActiveNode.end())
      mActiveNode[threadId] = mActiveNode[threadId]->addChild(methodName, threadId);
   else
      mActiveNode[threadId] = mRootNode->addChild(methodName, threadId);
}

void QBenchmarkRegisterer::startBenchmark(const std::string &methodName, const std::string &comment)
{
   std::lock_guard<std::mutex> lock { mMutex };

   std::stringstream ss;
   ss << std::this_thread::get_id();

   const auto threadId = ss.str();
   mActiveNode[threadId] = mActiveNode[threadId]->addChild(methodName, comment, threadId);
}

void QBenchmarkRegisterer::endBenchmark(const std::string &methodName)
{
   std::lock_guard<std::mutex> lock { mMutex };

   std::stringstream ss;
   ss << std::this_thread::get_id();
   const auto threadId = ss.str();

   if (mActiveNode[threadId]->getNodeName() == methodName)
   {
      mActiveNode[threadId]->close();
      mActiveNode[threadId] = mActiveNode[threadId]->getNextOpenChild();
   }
   else
   {
      auto node = mActiveNode[threadId]->searchParent(methodName);

      if (node)
      {
         node->close();
         mActiveNode[threadId] = node->getParent();
      }
   }
}

std::ostream &operator<<(std::ostream &out, const QBenchmarkRegisterer &node)
{
   out << (*node.mRootNode.get());

   return out;
}

}
