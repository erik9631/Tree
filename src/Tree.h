#pragma once
#include <execution>
#include <functional>
#include <future>

#include <vector>
#include <queue>
#include <memory>
#include <stack>
#include <ranges>

template <typename T>
struct Tree
{
    std::vector<std::unique_ptr<Tree<T>>> elementList;
    Tree<T>* parent = nullptr;
    Tree<T>* root = this;
    T* value = nullptr;
};

template <typename T>
std::unique_ptr<Tree<T>> CreateNode(T* value)
{
    auto node = std::make_unique<Tree<T>>();
    node->value = value;
    return node;
}

template <typename T>
std::unique_ptr<Tree<T>> CreateSubNode(Tree<T>& node, T* value)
{
    auto node = std::make_unique<Tree<T>>();
    node->value = value;
    return AddNode(node, value);
}


template <typename T>
bool IsRoot(Tree<T>& node)
{
    return node.root == &node;
}

template <typename T>
Tree<T>* AddNode(Tree<T>& node, std::unique_ptr<Tree<T>> nodeToAdd)
{
    auto nodeRef = nodeToAdd.get();
    node.elementList.push_back(std::move(nodeToAdd));
    nodeRef->parent = &node;
    nodeRef->root = node.root;
    return nodeRef;
}

template <typename T>
void Bfs (Tree<T>& tree, std::function<void(Tree<T>&)>& action)
{
    std::queue<Tree<T>*> nodeQueue;
    nodeQueue.push(&tree);
    while (!nodeQueue.empty())
    {
        Tree<T>* currentNode = nodeQueue.front();
        nodeQueue.pop();
        action(*currentNode);
        for (auto& node : currentNode->elementList)
            nodeQueue.push(node.get());
    }
}

// This is a bottom to top traversal
template <typename T>
void Dfs (Tree<T>& tree, const std::function<void(Tree<T>&)>& action)
{
    std::stack<std::tuple<Tree<T>*, std::ranges::iterator_t<std::vector<std::unique_ptr<Tree<T>>>>>> nodeStack;
    nodeStack.push({&tree, tree.elementList.begin()});
    while(!nodeStack.empty())
    {
        auto& [nodePtr, it] = nodeStack.top();
        if(it == std::ranges::end(nodePtr->elementList))
        {
            action(*nodePtr);
            nodeStack.pop();
            continue;
        }
        nodeStack.push({(*it).get(), it->get()->elementList.begin()});
        ++it;
    }
}

//This is a top to bottom traversal
template <typename T>
void ParallelTraversal (Tree<T>& tree, const std::function<void(Tree<T>&)>& action)
{
    std::for_each(std::execution::par, tree.elementList.begin(), tree.elementList.end(), [&action](auto& node)
    {
        action(*node);
        ParallelTraversal(*node, action);
    });
}

template <typename T>
void UpdateRoots(Tree<T>& root)
{
    if (!IsRoot(root))
        throw std::exception("This is not a root node");

    std::function<void(Tree<T>&)> action = [&root](Tree<T>& node)
    {
        node.root = &root;
    };
    ParallelTraversal(root, action);
}

template <typename T>
std::unique_ptr<Tree<T>> RemoveNode(Tree<T>& parentNode, Tree<T>& nodeToRemove)
{
    for (auto it = parentNode.elementList.begin(); it != parentNode.elementList.end(); it++)
    {
        if(it->get() != &nodeToRemove)
            continue;

        auto removedNode = std::move(*it);

        removedNode->parent = nullptr;
        removedNode->root = removedNode.get();
        parentNode.elementList.erase(it);
        UpdateRoots(*removedNode);
        return removedNode;
    }
    return nullptr;
}