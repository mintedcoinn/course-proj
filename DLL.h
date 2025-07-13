#pragma once
#include <iostream>
#include <QString>
#include <QQueue>
#include <QVariant>
template <typename T>
class DLL {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T value) : data(value), prev(nullptr), next(nullptr) {}
    };
    int node_count = 0;
    Node* head;
    Node* tail;
public:

    DLL() : head(nullptr), tail(nullptr), node_count(0){
    }
    ~DLL() { clear(); }
    QQueue<QString>loger;
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
            this->node_count = other.node_count;
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
        loger.enqueue(QString("DLL insert: insert value: %1").arg(QVariant::fromValue(value).toString()));
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
        loger.enqueue(QString("DLL clear: clear succsessfull"));
    }

    void removeValue(T value) {
        Node* current = head;
        bool removed= false;
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
                node_count--;
                loger.enqueue(QString("DLL remove: value: %1 removed").arg(QVariant::fromValue(value).toString()));
                removed = true;
            }
            else {
                current = current->next;
            }
        }
        if (!removed) loger.enqueue(QString("DLL remove: value: %1 not found").arg(QVariant::fromValue(value).toString()));
    }

    bool find(T value)  {
        Node* current = head;
        while (current) {
            if (current->data == value) {
                loger.enqueue(QString("DLL search: value: %1 found").arg(QVariant::fromValue(value).toString()));
                return true;
            }
            current = current->next;
        }
        loger.enqueue(QString("DLL search: value: %1 not found").arg(QVariant::fromValue(value).toString()));
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
