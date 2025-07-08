#include <iostream>
template <typename T>
class DLL {
private:
    struct Node {
        T data;
        Node<T>* prev;
        Node<T>* next;
        Node(T value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node<T>* head;
    Node<T>* tail;
public:

    DLL() : head(nullptr), tail(nullptr) {}
    ~DLL() { clear(); }
    DLL(const DLL& other) : head(nullptr), tail(nullptr) {
        Node<T>* current = other.head;
        while (current) {
            append(current->data);
            current = current->next;
        }
    }

    DLL& operator=(const DLL& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current) {
                append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void append(T value) {
        Node<T>* newNode = new Node(value);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void print() const {
        Node<T>* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void clear() {
        Node<T>* current = head;
        while (current) {
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
    }

    void removeValue(T value) {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) {
                Node<T>* toDelete = current;
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
    }

    bool find(T value) const {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};