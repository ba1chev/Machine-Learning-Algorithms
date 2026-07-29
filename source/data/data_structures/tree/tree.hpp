#pragma once

#include <utility>

#include "source/data/data_structures/tree/tree_node.hpp"

template <class T>
class Tree {
private:
    TreeNode<T>* root = nullptr;

    TreeNode<T>* clone_subtree(const TreeNode<T>* node) const;
    size_t count_subtree(const TreeNode<T>* node) const;
    void free_subtree(TreeNode<T>* node);

    void copy_from(const Tree& other);
    void move_to(Tree&& other) noexcept;
    void free();

public:
    Tree() = default;
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    Tree& operator = (const Tree& other);
    Tree& operator = (Tree&& other) noexcept;
    ~Tree();

    bool empty() const;
    size_t size() const;
    TreeNode<T>* get_root() const;

    TreeNode<T>* create_node(const T& value) const;
    void set_root(TreeNode<T>* node);
    void clear();
};

template <class T>
TreeNode<T>* Tree<T>::clone_subtree(const TreeNode<T>* node) const {
    if (!node) return nullptr;
    TreeNode<T>* copy = new TreeNode<T>(node->value);
    copy->left  = clone_subtree(node->left);
    copy->right = clone_subtree(node->right);
    return copy;
}

template <class T>
void Tree<T>::free_subtree(TreeNode<T>* node) {
    if (!node) return;
    free_subtree(node->left);
    free_subtree(node->right);
    delete node;
}

template <class T>
size_t Tree<T>::count_subtree(const TreeNode<T>* node) const {
    if (!node) return 0;
    return 1 + count_subtree(node->left) + count_subtree(node->right);
}

template <class T>
void Tree<T>::copy_from(const Tree& other) {
    root = clone_subtree(other.root);
}

template <class T>
void Tree<T>::move_to(Tree&& other) noexcept {
    root = other.root;
    other.root = nullptr;
}

template <class T>
void Tree<T>::free() {
    free_subtree(root);
    root = nullptr;
}

template <class T>
Tree<T>::Tree(const Tree& other) {
    copy_from(other);
}

template <class T>
Tree<T>::Tree(Tree&& other) noexcept {
    move_to(std::move(other));
}

template <class T>
Tree<T>& Tree<T>::operator = (const Tree<T>& other) {
    if (this != &other) {
        free();
        copy_from(other);
    }
    return *this;
}

template <class T>
Tree<T>& Tree<T>::operator = (Tree<T>&& other) noexcept {
    if (this != &other) {
        free();
        move_to(std::move(other));
    }
    return *this;
}

template <class T>
Tree<T>::~Tree() {
    free();
}

template <class T>
TreeNode<T>* Tree<T>::create_node(const T& value) const {
    return new TreeNode<T>(value);
}

template <class T>
void Tree<T>::set_root(TreeNode<T>* node) {
    free();
    root = node;
}

template <class T>
TreeNode<T>* Tree<T>::get_root() const {
    return root;
}

template <class T>
bool Tree<T>::empty() const {
    return root == nullptr;
}

template <class T>
size_t Tree<T>::size() const {
    return count_subtree(root);
}

template <class T>
void Tree<T>::clear() {
    free();
}
