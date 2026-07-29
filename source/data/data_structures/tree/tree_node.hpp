#pragma once

#include <utility>

template <class T>
struct TreeNode {
public:
    T value = T{};
    TreeNode* left  = nullptr;
    TreeNode* right = nullptr;

    TreeNode() = default;
    TreeNode(const T& value);
};

template <class T>
TreeNode<T>::TreeNode(const T& value): 
    value(value), left(nullptr), right(nullptr) {}