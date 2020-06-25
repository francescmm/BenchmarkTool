#include <catch.hpp>

#include <string>

#include <Node.h>

using namespace Benchmarker;

class MyTimeProvider : public ITimeProvider
{
public:
    MyTimeProvider() = default;
    std::chrono::microseconds getTimeSinceEpochMsecs() const override { return std::chrono::microseconds(0); }
};

TEST_CASE("Default values", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string nodeName = "Node1";
    const std::string threadId = "0";

    Node node(nodeName, threadId, nullptr, &timeProvider);

    REQUIRE(node.isClosed() == false);
    REQUIRE(node.getLevel() == 0);
    REQUIRE(node.getParent() == nullptr);
    REQUIRE(node.getNodeName() == nodeName);
    REQUIRE(node.getNextOpenChild() == &node);
    REQUIRE(node.getDuration() == -1);
}

TEST_CASE("Closed node", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string nodeName = "Node1";
    const std::string threadId = "0";

    Node node(nodeName, threadId, nullptr, &timeProvider);
    node.close();

    REQUIRE(node.isClosed() == true);
    REQUIRE(node.getLevel() == 0);
    REQUIRE(node.getParent() == nullptr);
    REQUIRE(node.getNodeName() == nodeName);
    REQUIRE(node.getNextOpenChild() == &node);
    REQUIRE(node.getDuration() == 0);
}

TEST_CASE("1 child with parent. Parent without childs.", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string nodeName = "Node2";
    const std::string threadId = "0";

    Node node("Node1", threadId, nullptr, &timeProvider);
    Node child(nodeName, threadId, &node, &timeProvider);

    REQUIRE(node.getLevel() == 0);

    REQUIRE(child.isClosed() == false);
    REQUIRE(child.getParent() == &node);
    REQUIRE(child.getNodeName() == nodeName);
    REQUIRE(child.getNextOpenChild() == &child);
    REQUIRE(child.getLevel() == 1);

    REQUIRE(node.getNextOpenChild()->getNodeName() == node.getNodeName());

    REQUIRE(child.getParent()->getNodeName() == node.getNodeName());
}

TEST_CASE("2 children with parent. Parent without child.", "[Node]")
{
    const std::string threadId = "0";
    MyTimeProvider timeProvider;

    Node node("Node1", threadId, nullptr, &timeProvider);
    Node child1("Child 1", threadId, &node, &timeProvider);
    Node child2("Child 2", threadId, &node, &timeProvider);

    REQUIRE(node.getLevel() == 0);
    REQUIRE(child1.getLevel() == 1);
    REQUIRE(child2.getLevel() == 1);

    REQUIRE(node.getNextOpenChild()->getNodeName() == node.getNodeName());

    REQUIRE(child1.getParent()->getNodeName() == node.getNodeName());
    REQUIRE(child2.getParent()->getNodeName() == node.getNodeName());
}

TEST_CASE("1 grandchild with parent", "[Node]")
{
    const std::string threadId = "0";
    MyTimeProvider timeProvider;

    Node node("Node1", threadId, nullptr, &timeProvider);
    const auto child1 = node.addChild("Child 1", "", threadId);
    const auto child2 = child1->addChild("Child 2", "", threadId);

    REQUIRE(node.getLevel() == 0);
    REQUIRE(child1->getLevel() == 1);
    REQUIRE(child2->getLevel() == 2);

    REQUIRE(node.getNextOpenChild()->getNodeName() == child1->getNodeName());
    REQUIRE(child1->getNextOpenChild()->getNodeName() == child2->getNodeName());
}

TEST_CASE("Getting 2 open relatives", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string threadId = "0";

    Node node("Node1", threadId, nullptr, &timeProvider);
    const auto child1 = node.addChild("Child 1", "", threadId);
    const auto uncle1 = node.addChild("Uncle 1", "", threadId);
    const auto uncle2 = node.addChild("Uncle 2", "", threadId);
    const auto child3 = child1->addChild("Child 3", "", threadId);

    REQUIRE(node.getRelativeByName("") == nullptr);
    REQUIRE(child3->getRelativeByName("Node") == nullptr);
    REQUIRE(child3->getRelativeByName("Uncle 1") == uncle1);
    REQUIRE(child3->getRelativeByName("Uncle 2") == uncle2);
}

TEST_CASE("Getting 1 open relative", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string threadId = "0";

    Node node("Node1", threadId, nullptr, &timeProvider);
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

TEST_CASE("Node output", "[Node]")
{
    MyTimeProvider timeProvider;
    const std::string threadId = "0";

    Node node("Node1", threadId, nullptr, &timeProvider);
    node.addChild("Child 1", "", threadId);

    node.close();

    std::string expectedOutput = "[0] {Node1} done in {0.000000} msec.\n  [0] {Child 1} done in {0.000000} msec. - Force closed\n";
    std::string realOutput;
    realOutput << node;

    REQUIRE(expectedOutput == realOutput);
}
