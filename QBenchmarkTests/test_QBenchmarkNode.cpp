#include <catch.hpp>

#include <string>

#include <QBenchmarkNode.h>

using namespace QBenchmark;

TEST_CASE("Default values", "[QBenchmarkNode]")
{
    const std::string nodeName = "Node1";
    const std::string threadId = "0";

    QBenchmarkNode node(nodeName, threadId, nullptr);

    REQUIRE(node.isClosed() == false);
    REQUIRE(node.getLevel() == 0);
    REQUIRE(node.getParent() == nullptr);
    REQUIRE(node.getNodeName() == nodeName);
    REQUIRE(node.getNextOpenChild() == &node);
}

TEST_CASE("Closed node", "[QBenchmarkNode]")
{
    const std::string nodeName = "Node1";
    const std::string threadId = "0";

    QBenchmarkNode node(nodeName, threadId, nullptr);
    node.close();

    REQUIRE(node.isClosed() == true);
    REQUIRE(node.getLevel() == 0);
    REQUIRE(node.getParent() == nullptr);
    REQUIRE(node.getNodeName() == nodeName);
    REQUIRE(node.getNextOpenChild() == &node);
}

TEST_CASE("1 child with parent. Parent without childs.", "[QBenchmarkNode]")
{
    const std::string nodeName = "Node2";
    const std::string threadId = "0";

    QBenchmarkNode node("Node1", threadId, nullptr);
    QBenchmarkNode child(nodeName, threadId, &node);

    REQUIRE(node.getLevel() == 0);

    REQUIRE(child.isClosed() == false);
    REQUIRE(child.getParent() == &node);
    REQUIRE(child.getNodeName() == nodeName);
    REQUIRE(child.getNextOpenChild() == &child);
    REQUIRE(child.getLevel() == 1);

    REQUIRE(node.getNextOpenChild()->getNodeName() == node.getNodeName());

    REQUIRE(child.getParent()->getNodeName() == node.getNodeName());
}

TEST_CASE("2 children with parent. Parent without child.", "[QBenchmarkNode]")
{
    const std::string threadId = "0";

    QBenchmarkNode node("Node1", threadId, nullptr);
    QBenchmarkNode child1("Child 1", threadId, &node);
    QBenchmarkNode child2("Child 2", threadId, &node);

    REQUIRE(node.getLevel() == 0);
    REQUIRE(child1.getLevel() == 1);
    REQUIRE(child2.getLevel() == 1);

    REQUIRE(node.getNextOpenChild()->getNodeName() == node.getNodeName());

    REQUIRE(child1.getParent()->getNodeName() == node.getNodeName());
    REQUIRE(child2.getParent()->getNodeName() == node.getNodeName());
}

TEST_CASE("1 grandchild with parent", "[QBenchmarkNode]")
{
    const std::string threadId = "0";

    QBenchmarkNode node("Node1", threadId, nullptr);
    const auto child1 = node.addChild("Child 1", "", threadId);
    const auto child2 = child1->addChild("Child 2", "", threadId);

    REQUIRE(node.getLevel() == 0);
    REQUIRE(child1->getLevel() == 1);
    REQUIRE(child2->getLevel() == 2);

    REQUIRE(node.getNextOpenChild()->getNodeName() == child1->getNodeName());
    REQUIRE(child1->getNextOpenChild()->getNodeName() == child2->getNodeName());
}

TEST_CASE("Getting 2 open relatives", "[QBenchmarkNode]")
{
    const std::string threadId = "0";

    QBenchmarkNode node("Node1", threadId, nullptr);
    const auto child1 = node.addChild("Child 1", "", threadId);
    const auto uncle1 = node.addChild("Uncle 1", "", threadId);
    const auto uncle2 = node.addChild("Uncle 2", "", threadId);
    const auto child3 = child1->addChild("Child 3", "", threadId);

    REQUIRE(node.getRelativeByName("") == nullptr);
    REQUIRE(child3->getRelativeByName("Node") == nullptr);
    REQUIRE(child3->getRelativeByName("Uncle 1") == uncle1);
    REQUIRE(child3->getRelativeByName("Uncle 2") == uncle2);
}

TEST_CASE("Getting 1 open relative", "[QBenchmarkNode]")
{
    const std::string threadId = "0";

    QBenchmarkNode node("Node1", threadId, nullptr);
    const auto child1 = node.addChild("Child 1", "", threadId);
    const auto uncle1 = node.addChild("Uncle 1", "", threadId);
    const auto uncle2 = node.addChild("Uncle 2", "", threadId);
    const auto child3 = child1->addChild("Child 3", "", threadId);

    uncle1->close();

    REQUIRE(node.getRelativeByName("") == nullptr);
    REQUIRE(child3->getRelativeByName("Node") == nullptr);
    REQUIRE(child3->getRelativeByName("Uncle 1") == nullptr);
    REQUIRE(child3->getRelativeByName("Uncle 2") == uncle2);
}
