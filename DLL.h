#pragma once
#include <iostream>
template <typename T>
class DLL {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T value) : data(value), prev(nullptr), next(nullptr) {}
    };
    int node_count;
    Node* head;
    Node* tail;
public:

    DLL() : head(nullptr), tail(nullptr), node_count(0){
    }
    ~DLL() { clear(); }
    DLL(const DLL& other) : head(nullptr), tail(nullptr) {
        Node* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
    }

    DLL& operator=(const DLL& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void append(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        node_count++;
    }

    void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        node_count = 0;
    }

    void removeValue(T value) {
        Node* current = head;
        while (current) {
            if (current->data == value) {
                Node* toDelete = current;
                if (current->prev) {
                    current->prev->next = current->next;
                }
                else {
                    head = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                else {
                    tail = current->prev;
                }
                current = current->next;
                delete toDelete;
            }
            else {
                current = current->next;
            }
        }
        node_count--;
    }

    bool find(T value) const {
        Node* current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    int getNodeCount(){
        return node_count;
    }
    Node* getHead(){
        return head;
    }

    bool isEmpty(){
        return node_count==0;
    }
};
