//
// Created by erik9 on 3/10/2024.
//
#include <iostream>
#include <unordered_set>

#include "catch_amalgamated.hpp"
#include "Tree.h"

TEST_CASE("Basic tree check", "[Value check]")
{
    auto tree1 = std::make_unique<Tree<int>>();
    auto tree2 = std::make_unique<Tree<int>>();
    int a = 5;
    int b = 4;
    tree1->value = &a;
    tree2->value = &b;

    auto tree2Ref = tree2.get();

    AddNode(*tree1, std::move(tree2));

    REQUIRE(*tree1->value == 5);
    REQUIRE(*tree2Ref->value == 4);
    REQUIRE(tree1->elementList.size() == 1);
    REQUIRE(tree2Ref->parent == tree1.get());
}

TEST_CASE("Tree depth check", "[Depth check]")
{
    auto tree1 = CreateNode(new int(1));
    auto tree12 = CreateNode(new int(12));
    auto tree13 = CreateNode(new int(13));
    auto tree121 = CreateNode(new int(121));
    auto tree122 = CreateNode(new int(122));
    auto tree123 = CreateNode(new int(123));

    REQUIRE(tree1->root == tree1.get());

    auto tree12Ref = AddNode(*tree1, std::move(tree12));
    auto tree13Ref = AddNode(*tree1, std::move(tree13));
    auto tree121Ref = AddNode(*tree12Ref, std::move(tree121));
    auto tree122Ref = AddNode(*tree12Ref, std::move(tree122));
    auto tree123Ref = AddNode(*tree12Ref, std::move(tree123));

    REQUIRE(*tree1->value == 1);
    REQUIRE(*tree12Ref->value == 12);
    REQUIRE(*tree13Ref->value == 13);
    REQUIRE(*tree121Ref->value == 121);
    REQUIRE(*tree122Ref->value == 122);
    REQUIRE(*tree123Ref->value == 123);

    // Checking the tree structure
    REQUIRE(tree1->elementList.size() == 2);
    REQUIRE(tree12Ref->elementList.size() == 3);
    REQUIRE(tree13Ref->elementList.empty());
    REQUIRE(tree121Ref->elementList.empty());
    REQUIRE(tree122Ref->elementList.empty());
    REQUIRE(tree123Ref->elementList.empty());

    REQUIRE(tree12Ref->parent == tree1.get());
    REQUIRE(tree13Ref->parent == tree1.get());
    REQUIRE(tree121Ref->parent == tree12Ref);
    REQUIRE(tree122Ref->parent == tree12Ref);
    REQUIRE(tree123Ref->parent == tree12Ref);

    // Checking roots
    REQUIRE(IsRoot(*tree1));
    REQUIRE(!IsRoot(*tree12Ref));
    REQUIRE(!IsRoot(*tree13Ref));
    REQUIRE(!IsRoot(*tree121Ref));
    REQUIRE(!IsRoot(*tree122Ref));
    REQUIRE(!IsRoot(*tree123Ref));

    REQUIRE(tree1->root == tree1.get());
    REQUIRE(tree12Ref->root == tree1.get());
    REQUIRE(tree13Ref->root == tree1.get());
    REQUIRE(tree121Ref->root == tree1.get());
    REQUIRE(tree122Ref->root == tree1.get());
    REQUIRE(tree123Ref->root == tree1.get());
}

TEST_CASE("Tree remove check", "[Remove check]")
{
    auto tree1 = CreateNode(new int(1));
    auto tree12 = CreateNode(new int(12));
    auto tree13 = CreateNode(new int(13));
    auto tree121 = CreateNode(new int(121));
    auto tree122 = CreateNode(new int(122));
    auto tree123 = CreateNode(new int(123));

    auto tree12Ref = AddNode(*tree1, std::move(tree12));
    auto tree13Ref = AddNode(*tree1, std::move(tree13));
    auto tree121Ref = AddNode(*tree12Ref, std::move(tree121));
    auto tree122Ref = AddNode(*tree12Ref, std::move(tree122));
    auto tree123Ref = AddNode(*tree12Ref, std::move(tree123));

    SECTION("Remove leaf")
    {
        auto removedNode = RemoveNode(*tree12Ref, *tree123Ref);

        REQUIRE(tree12Ref->elementList.size() == 2);
        REQUIRE(tree123Ref->parent == nullptr);
        REQUIRE(tree123Ref->root == tree123Ref);
        REQUIRE(*removedNode->value == 123);
        REQUIRE(removedNode->parent == nullptr);
        REQUIRE(removedNode->root == removedNode.get());

    }

    SECTION("Remove non leaf")
    {
        auto removedNode = RemoveNode(*tree1, *tree12Ref);

        REQUIRE(tree1->elementList.size() == 1);
        REQUIRE(tree12Ref->parent == nullptr);
        REQUIRE(tree12Ref->root == tree12Ref);
        REQUIRE(*removedNode->value == 12);
        REQUIRE(removedNode->parent == nullptr);
        REQUIRE(removedNode->root == removedNode.get());

        //Verify tree integrity
        REQUIRE(tree1->elementList.size() == 1);
        REQUIRE(tree13Ref->parent == tree1.get());
        REQUIRE(tree13Ref->root == tree1.get());

        REQUIRE(tree12Ref->elementList.size() == 3);
        REQUIRE(tree12Ref->parent == nullptr);
        REQUIRE(tree12Ref->root == tree12Ref);

        REQUIRE(tree121Ref->parent == tree12Ref);
        REQUIRE(tree121Ref->root == tree12Ref);

        REQUIRE(tree122Ref->parent == tree12Ref);
        REQUIRE(tree122Ref->root == tree12Ref);

        REQUIRE(tree123Ref->parent == tree12Ref);
        REQUIRE(tree123Ref->root == tree12Ref);
    }
}

TEST_CASE("BFS traveral test", "[BFS Test]")
{
    auto tree1 = CreateNode(new int(1));
    auto tree12 = CreateNode(new int(12));
    auto tree13 = CreateNode(new int(13));
    auto tree121 = CreateNode(new int(121));
    auto tree122 = CreateNode(new int(122));
    auto tree123 = CreateNode(new int(123));

    auto tree12Ref = AddNode(*tree1, std::move(tree12));
    AddNode(*tree1, std::move(tree13));
    AddNode(*tree12Ref, std::move(tree121));
    AddNode(*tree12Ref, std::move(tree122));
    AddNode(*tree12Ref, std::move(tree123));

    std::vector<int> values = {1, 12, 13, 121, 122, 123};
    auto it = values.begin();

    std::function<void(Tree<int>&)> action = [&values, &it](Tree<int>& node) {
        REQUIRE(*node.value == *it);
        std::cout << *node.value << std::endl;
        ++it;
    };
    Bfs(*tree1, action);

}

TEST_CASE("DFS traversal test", "[DFS Test]")
{
    auto tree1 = CreateNode(new int(1));
    auto tree12 = CreateNode(new int(12));
    auto tree13 = CreateNode(new int(13));
    auto tree121 = CreateNode(new int(121));
    auto tree122 = CreateNode(new int(122));
    auto tree123 = CreateNode(new int(123));

    auto tree12Ref = AddNode(*tree1, std::move(tree12));
    AddNode(*tree1, std::move(tree13));
    AddNode(*tree12Ref, std::move(tree121));
    AddNode(*tree12Ref, std::move(tree122));
    AddNode(*tree12Ref, std::move(tree123));

    std::vector<int> values = {121, 122, 123, 12, 13, 1};
    auto it = values.begin();

    std::function<void(Tree<int>&)> action = [&values, &it](Tree<int>& node) {
        REQUIRE(*node.value == *it);
        std::cout << *node.value << std::endl;
        ++it;
    };
    Dfs(*tree1, action);
}

TEST_CASE("Parallel traversal", "[Parallel Traversal test]")
{
    auto tree1 = CreateNode(new int(1));
    auto tree12 = CreateNode(new int(12));
    auto tree13 = CreateNode(new int(13));
    auto tree121 = CreateNode(new int(121));
    auto tree122 = CreateNode(new int(122));
    auto tree123 = CreateNode(new int(123));

    auto tree12Ref = AddNode(*tree1, std::move(tree12));
    AddNode(*tree1, std::move(tree13));
    AddNode(*tree12Ref, std::move(tree121));
    AddNode(*tree12Ref, std::move(tree122));
    AddNode(*tree12Ref, std::move(tree123));

    std::unordered_set<int> keys = {1, 12, 13, 121, 122, 123};

    std::function<void(Tree<int>&)> action = [&keys](Tree<int>& node)
    {
        REQUIRE(keys.find(*node.value) != keys.end());
    };
    ParallelTraversal(*tree1, action);
}