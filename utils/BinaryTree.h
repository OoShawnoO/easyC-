#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include <stack>
#include <vector>
#include <deque>
#include "sort.h"

#define POP(node,stack) do{                                     \
                            (node) = (stack).top();             \
                            (stack).pop();                      \
                        }while(0)

#define PUSH_LEFT(node,stack) do{                               \
                                if(node->left)                  \
                                {                               \
                                    stack.push(node->left);     \
                                }                               \
                            }while(0)

#define PUSH_RIGHT(node,stack) do{                               \
                                    if(node->right)              \
                                    {                            \
                                        stack.push(node->right); \
                                    }                            \
                                }while(0)

using namespace std;

enum Color
{
    RED,
    BLACK
};

template <typename T>
struct BinaryTreeNode{
    BinaryTreeNode* parent;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    T data;
    Color color;
public:
    explicit BinaryTreeNode(T _data,
                            Color _color = RED,
                            BinaryTreeNode* _parent = nullptr,
                            BinaryTreeNode* _left = nullptr,
                            BinaryTreeNode* _right = nullptr):
            data(_data),color(_color),parent(_parent),left(_left),right(_right){}
};


template <typename T>
class BinaryTree
{
private:
    size_t BinaryTreeHeight(BinaryTreeNode<T>* node)
    {
        if(!node) return 0;
        return BinaryTreeHeight(node->left) > BinaryTreeHeight(node->right) ?
        BinaryTreeHeight(node->left) + 1 :
        BinaryTreeHeight(node->right) + 1;
    }

    size_t BinaryTreeLeaves(BinaryTreeNode<T>* node)
    {
        size_t count;
        if(node == nullptr) count = 0;
        else if(node->right == nullptr && node->left == nullptr) count = 1;
        else{
            count = BinaryTreeLeaves(node->right) + BinaryTreeLeaves(node->left);
        }
        return count;
    }
protected:
    BinaryTreeNode<T>* root{};
    size_t size;
public:
    BinaryTree(){
        root = nullptr;
        size = 0;
    }
    explicit BinaryTree(T _data){
        root = new BinaryTreeNode<T>(_data);
        size = 1;
    }
    ~BinaryTree(){
        delete root;
    }

    inline virtual size_t getSize(){return size;}
    virtual size_t getDepth()
    {
        return BinaryTreeHeight(root);
    }
    virtual size_t getLeaves()
    {
        return BinaryTreeLeaves(root);
    }

    virtual vector<T> PreOrderTraverse()
    {
        if(root == nullptr) return vector<T>();
        vector<T> vec;
        stack<BinaryTreeNode<T>*> stack;
        stack.push(root);
        BinaryTreeNode<T>* node;
        while(!stack.empty()){
            POP(node,stack);
            vec.push_back(node->data);
            PUSH_RIGHT(node,stack);
            PUSH_LEFT(node,stack);
        }
        return vec;
    }
    virtual vector<T> InOrderTraverse()
    {
        if(root == nullptr) return vector<T>();
        vector<T> vec;
        stack<BinaryTreeNode<T>*> stack;
        auto cur = root;
        while(cur != nullptr || !stack.empty())
        {
            if(cur != nullptr)
            {
                stack.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = stack.top();
                stack.pop();
                vec.push_back(cur->data);
                cur = cur->right;
            }
        }
        return vec;
    }
    virtual vector<T> PostOrderTraverse()
    {
        if(root == nullptr) return vector<T>();
        vector<T> vec;
        stack<BinaryTreeNode<T>*> stack;
        stack.push(root);
        BinaryTreeNode<T>* node;
        while(!stack.empty()){
            POP(node,stack);
            vec.push_back(node->data);
            PUSH_LEFT(node,stack);
            PUSH_RIGHT(node,stack);
        }
        return vec;
    }
    virtual vector<T> LevelTraverse()
    {
        if(root == nullptr) return vector<T>();
        vector<T> vec;
        deque<BinaryTreeNode<T>*> queue;
        queue.push_back(root);
        while(queue.size() > 0)
        {
            auto node = queue.front();
            queue.pop_front();
            vec.push_back(node->data);
            if(node->left) queue.push_back(node->left);
            if(node->right) queue.push_back(node->right);
        }
        return vec;
    }
};

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
private:

public:
    explicit BinarySearchTree(T _data): BinaryTree<T>(_data){}
    ~BinarySearchTree() = default;
    bool Contains(T _data)
    {
        vector<T> vec = BinaryTree<T>::InOrderTraverse();
        return binary_search(vec.begin(),vec.end(),_data);
    }
    bool Add(T _data,bool (*Compare)(T,T) = Greater)
    {
        if(Contains(_data)) return false;
        auto node = new BinaryTreeNode<T>(_data);
        auto cur = BinaryTree<T>::root;
        while(cur != nullptr)
        {
            if(Compare(_data,cur->data))
            {
                if(cur->right)
                {
                    cur = cur->right;
                }
                else break;
            }
            else
            {
                if(cur->left)
                {
                    cur = cur->left;
                }
                else break;
            }
        }
        if(Compare(_data,cur->data))
        {
            cur->right = node;
        }
        else
        {
            cur->left = node;
        }
        node->parent = cur;
        BinaryTree<T>::size++;
        return true;
    }
    BinaryTreeNode<T>* Find(T _data,bool (*Compare)(T,T) = Greater)
    {
        if(!BinaryTree<T>::root) return nullptr;
        if(!Contains(_data)) return nullptr;
        auto cur = BinaryTree<T>::root;
        while(cur->data != _data)
        {
            if(Compare(_data,cur->data))
            {
                cur = cur->right;
            }
            else
            {
                cur = cur->left;
            }
        }
        return cur;
    }
    bool Delete(T _data,bool (*Compare)(T,T) = Greater)
    {
        BinaryTreeNode<T>* cur;
        if(!(cur = Find(_data,Compare))) return false;
        auto par = cur->parent;
        if(!cur->left && !cur->right)
        {

        }
        else if(!cur->left && cur->right)
        {

            if(!par) BinaryTree<T>::root = cur->right;
            else{
                (par->left == cur ? par->left : par->right) = cur->right;
            }
        }
        else if(!cur->right && cur->left)
        {
            if(!par) BinaryTree<T>::root = cur->left;
            else
            {
                (par->right == cur ? par->right : par->left) = cur->left;
            }
        }
        else
        {
            auto node = cur->right;
            if(!par) BinaryTree<T>::root = cur->right;
            else
            {
                (par->right == cur ? par->right : par->left) = cur->right;
            }
            while(node->left){
                node = node->left;
            }
            node->left = cur->left;

        }
        delete cur;
        BinaryTree<T>::size--;
        return true;
    }
};

template <typename T>
class RB_BinaryTree : public BinarySearchTree<T>
{
public:

};

#endif
