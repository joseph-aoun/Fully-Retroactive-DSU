//
// Created by Joseph on 4/28/2024.
//
#include<iostream>
#include<vector>
using namespace std;

struct Node {
    int data; // key of the node
    Node *parent;
    Node *left;
    Node *right;
};

typedef Node * nodePtr;

class SplayTree {
private:
    nodePtr root;
    nodePtr Search_(nodePtr r, int val) {
        if(!r) return r;
        if(r->data == val) return r;
        if(r->data < val) return Search_(r->right, val);
        else return Search_(r->left, val);
    }
    void leftRotate(nodePtr node) {
        nodePtr rightChild = node->right;
        node->right = rightChild->left;

        if(rightChild->left)
            rightChild->left->parent = node; // if rightChild has a left node, make its parent the node

        rightChild->parent = node->parent; // update the parent of the rightChild a.k.a lift it up
        if(!rightChild->parent) root = rightChild;
        else if (node == node->parent->left) node->parent->left = rightChild;
        else node->parent->right = rightChild;

        node->parent = rightChild; // finish rotation
        rightChild->left = node;
    }

    void rightRotate(nodePtr node) {
        nodePtr leftChild = node->left;
        node->right = leftChild->right;

        if(leftChild->right) leftChild->right->parent = node;

        leftChild->parent = node->parent;
        if(!node->parent) root = leftChild;
        else if(node == node->parent->left) node->parent->left = leftChild;
        else node->parent->left = leftChild;
    }

    void splay(nodePtr node) {
        while (node->parent) {
            if (!node->parent->parent) {
                if(node == node->parent->left) rightRotate(node->parent);
                else leftRotate(node->parent);
            } else if(node == node->parent->left && node->parent == node->parent->parent->left) {
                rightRotate(node->parent->parent);
                rightRotate(node->parent);
            } else if(node == node->parent->right && node->parent == node->parent->parent->right) {
                leftRotate(node->parent->parent);
                leftRotate(node->parent);
            } else if(node == node->parent->right && node->parent == node->parent->parent->left) {
                leftRotate(node->parent);
                rightRotate(node->parent);
            } else {
                rightRotate(node->parent);
                leftRotate(node->parent);
            }
        }
    } // splay takes a node and moves it to the top to become the root





public:

};

int main() {
    return 0;
}