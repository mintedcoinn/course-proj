#pragma once
#include "DLL.h"
#include <queue>
template <typename K, typename V>
class AVLTree {
private:
    struct AVLNode {
        K key;
        DLL<V> list;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;
        int height;

        AVLNode(K k, V value, AVLNode* p = nullptr)
            : key(k), list(), left(nullptr), right(nullptr), parent(p), height(1) {
            list.append(value);
        }
    };

    AVLNode* root;

    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }

    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }

    int getBalance(AVLNode* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->parent = x;

        y->left = T2;
        if (T2) T2->parent = y;

        if (y == root) root = x;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->parent = y;

        x->right = T2;
        if (T2) T2->parent = x;

        if (x == root) root = y;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLNode* insert(AVLNode* node, K key, V value, AVLNode* parent = nullptr) {
        if (!node) {
            return  new AVLNode(key, value, parent);
        }

        if (key < node->key) {
            node->left = insert(node->left, key, value, node);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, value, node);
        }
        else {
            node->list.append(value);
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }
        // Right Right
        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }
        // Left Right
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Right Left
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* findMin(AVLNode* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    AVLNode* findMax(AVLNode* node) const {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    AVLNode* removeN(AVLNode* node, K key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeN(node->left, key);
        }
        else if (key > node->key) {
            node->right = removeN(node->right, key);
        }
        else {
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;

                if (!temp) {
                    delete node;
                    return nullptr;
                }
                else {
                    temp->parent = node->parent;
                    delete node;
                    return temp;
                }
            }
            else {
                AVLNode* successor = findMin(node->right);

                node->key = successor->key;
                node->list = successor->list;

                node->right = removeN(node->right, successor->key);
            }
        }
         updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

        AVLNode* removeV(AVLNode* node, K key, V value) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeV(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = removeV(node->right, key, value);
        }
        else {
            if (!node->list.find(value)) return node;
            else
            {
                node->list.removeValue(value);
                if (node->list.getNodeCount()!=0) return node;
                else
                {
                    if (!node->left || !node->right) {
                        AVLNode* temp = node->left ? node->left : node->right;

                        if (!temp) {
                            delete node;
                            return nullptr;
                        }
                        else {
                            temp->parent = node->parent;
                            delete node;
                            return temp;
                        }
                    }
                    else {
                        AVLNode* successor = findMin(node->right);

                        node->key = successor->key;
                        node->list = successor->list;

                        node->right = removeN(node->right, successor->key);
                        }
                    }
            }
        }

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void clear(AVLNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(root); }

    void insertValue(K key, V value) {
        root = insert(root, key, value, nullptr);
    }

    void removeNode(K key) {
        root = removeN(root, key);
    }
    
    void removeValue(K key, V value){
        root = removeV(root, key, value);
    }

    bool find(K key, DLL<V> &value, int* step_counter=nullptr) const {
        AVLNode* current = root;
        if (step_counter) *step_counter = 0;
        while (current) {
            if (step_counter) (*step_counter)++;
            if (key < current->key) {
                current = current->left;
            }
            else if (key > current->key) {
                current = current->right;
            }
            else {
                value = current->list;
                return true;
            }
        }
        return false;
    }

    void checkAndDeleteValueInTree(V value){
        if (!root) return;
        queue<AVLNode*> q;
        q.push(root);
        while (!q.empty()){
                AVLNode* current = q.front();
                q.pop();
                if (current->list.find(value)){
                    removeValue(current->key, value);
                    return;
                }

                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
        }
    }

    bool contains(K key) const {
        return find(key) != nullptr;
    }

    void printTree() const {
        if (!root) return;

        queue<AVLNode*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            while (levelSize--) {
                AVLNode* current = q.front();
                q.pop();

                std::cout << "[" << current->key << "](h:" << current->height << ") ";

                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            std::cout << std::endl;
        }
    }
    K getRootKey() const {
        return root ? root->key : K{};
    }
};

