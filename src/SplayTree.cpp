//
// Created by Joseph on 4/28/2024.
//
#include<iostream>
#include<vector>
#include<cassert>
using namespace std;

struct Node {
    int data{}; // key of the node
    Node *parent;
    Node *left;
    Node *right;
    Node() {
        parent = left = right = nullptr;
    }
};

typedef Node * nodePtr;

class SplayTree {
private:
    nodePtr root;
    nodePtr Search_(nodePtr r, int val, bool c = false) {
        if(!r) { return r; }
        if(!r->left && val < r->data && c) {
            auto node = new Node(); node->data = val; node->parent = r;
            return r->left = node;
        }
        if(!r->right && val >= r->data && c) {
            auto node = new Node(); node->data = val; node->parent = r;
            return r->right = node;
        }
        if(r->data == val && !c) return r;
        if(r->data <= val) return Search_(r->right, val, c);
        else { if(val == 3) cout << r->left->data << endl; return Search_(r->left, val, c); }
    }

    void leftRotate(nodePtr node) {
        nodePtr rightChild = node->right;
        node->right = rightChild->left;

        if(rightChild->left)
            rightChild->left->parent = node; // if rightChild has a left node, make its parent the node

        rightChild->parent = node->parent; // update the parent of the rightChild a.k.a lift it up
        if(!node->parent) root = rightChild;
        else if (node == node->parent->left) node->parent->left = rightChild;
        else node->parent->right = rightChild;

        node->parent = rightChild; // finish rotation
        rightChild->left = node;
    }

    void rightRotate(nodePtr node) {
        nodePtr leftChild = node->left;
        node->left  = leftChild->right;

        if(leftChild->right) leftChild->right->parent = node;

        leftChild->parent = node->parent;
        if(!node->parent) this->root = leftChild;
        else if(node == node->parent->left) node->parent->left = leftChild;
        else node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
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

    pair<nodePtr, nodePtr> split(nodePtr node) {
        splay(node);
        nodePtr s, t;
        if(node->right) {
            // if it is not the maximum element
            t = node->right; t->parent = nullptr;
        } else t = nullptr;
        s = node; s->right = node = nullptr;
        return {s, t};
    }

    nodePtr join(nodePtr s, nodePtr t) {
        // joins s and t, where all the keys in s are smaller than those in t
        if(!s) return t;
        if(!t) return s;

        nodePtr m_ = max(s);
        splay(m_);
        m_->right = t;
        t->parent = m_;

        return m_;
    }

    void deleteNode_(int val) {
        auto node = Search_(this->root, val);
        assert(node != nullptr);

        auto [s, t] = split(node);
        if(s->left) s->left->parent = nullptr;

        root = join(s->left, t); s = nullptr; delete(s);
    }

public:
    SplayTree() { this->root = nullptr; }

    nodePtr max(nodePtr node) {
        while(node->right) node = node->right; return node;
    }

    nodePtr min(nodePtr node) {
        while(node->left) node = node->left; return node;
    }

    nodePtr search(int key) {
        nodePtr node = Search_(root, key); if(node) splay(node);
        return node;
    }

    void insert(int key) {
        if(!root) { root = new Node(); root->data = key; return;}
        auto node = Search_(root, key, true);
        splay(node);
    }

    nodePtr predecessor(int key) {
        // assumes key is in the tree
        nodePtr node = Search_(root, key);
        if(node->left) return max(node->left);
        while(node->parent && node == node->parent->left) node = node->parent;
        return node->parent;
    }

    nodePtr successor(int key) {
        // also assumes that key is in the tree
        nodePtr node = Search_(root, key);
        if(node->right) return min(node->right);
        while(node->parent && node == node->parent->right) node = node->parent;
        return node->parent;
    }

    void deleteNode(int key) { deleteNode_(key); }
};

void test() {

    SplayTree splayTree;
    splayTree.insert(1);
    splayTree.insert(2);
    splayTree.insert(4);
    splayTree.insert(3);
    splayTree.insert(5);
    splayTree.insert(10);
    splayTree.insert(6);
    cout << splayTree.predecessor(6)->data << "\n";
    splayTree.deleteNode(5);
    cout << splayTree.predecessor(6)->data << "\n";
    cout << splayTree.successor(6)->data << "\n";
}

int main() {
    test();
    return 0;
}