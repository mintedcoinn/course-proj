#pragma once
#include <iostream> 
#include <string>
#include <fstream>
#include <algorithm>
#include <type_traits>
#include <QString>
#include <QVariant>
#include <QQueue>

using namespace std;
template <typename K, typename V>
class HashTable {

    struct Entry {
        K key;
        V value;
        int status;
    };

    Entry* table;
    int size;

    unsigned int hash(const K& key) const {
        unsigned long long h = 0;
        QString temp = QVariant::fromValue(key).toString();
        for (const QChar& c : temp) {
            h = h * 31 + static_cast<unsigned long long>(c.unicode());
        }
        h *= h;
        string s = to_string(h);
        int mid = s.length() / 2;
        string part = s.substr(mid - 1, 3);
        return stoi(part) % size;
    }

    int line_adresation(int hash, int attempt) const {
        return (hash + attempt) % size;
    }

public:
    HashTable(unsigned int sz = 0) : size(sz) {

        table = new Entry[size];

        for (unsigned int i = 0; i < size; ++i) {
            table[i].status = 0;
        }
    }

    ~HashTable() {
        delete[] table;
    }

    HashTable(const HashTable& other) : size(other.size) {
        table = new Entry[size];
        for (int i = 0; i < size; ++i) {
            table[i] = other.table[i];
        }
    }

    HashTable& operator=(HashTable other) {
        swap(*this, other);
        return *this;
    }

    HashTable(HashTable&& other) noexcept : size(0), table(nullptr) {
        swap(*this, other);
    }

    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            delete[] table;
            size = 0;
            table = nullptr;
            swap(*this, other);
        }
        return *this;
    }

    friend void swap(HashTable& first, HashTable& second) noexcept {
        using std::swap;
        swap(first.size, second.size);
        swap(first.table, second.table);
    }

    QQueue<QString>loger;

    bool insert(const V& value, const K& key) {
        unsigned int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                table[idx] = {key, value, 1 };
                loger.enqueue(QString("HashTable insert: insert key: %1 value: %2").arg(QVariant::fromValue(key).toString()).arg(QVariant::fromValue(value).toString()));
                return true;
            }

            if (table[idx].key == key) {
                loger.enqueue(QString("HashTable insert: key %1 already exist").arg(QVariant::fromValue(key).toString()));
                return false;
            }
        }
         loger.enqueue(QString("HashTable insert: insert failure (table overload)").arg(QVariant::fromValue(key).toString()));
        return false;
    }

    bool search(K& key, V* found_value = nullptr, int* steps= nullptr) {
        unsigned int h = hash(key);
        if (steps) *steps = 0;
         if (found_value) *found_value = -1;
        for (int i = 0; i < size; ++i) {
            if (steps)*steps++;
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                loger.enqueue(QString("HashTable search: key: %1 not found").arg(QVariant::fromValue(key).toString()));
                return false;
            }
            if (table[idx].status == 1 && table[idx].key == key) {
                if (found_value) *found_value = table[idx].value;
                 loger.enqueue(QString("HashTable search: key: %1 found").arg(QVariant::fromValue(key).toString()));
                return true;
            }
        }
        loger.enqueue(QString("HashTable search: key: %1 not found").arg(QVariant::fromValue(key).toString()));
        return false;
    }

    bool remove(K& key, V&value) {
        unsigned int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                 loger.enqueue(QString("HashTable remove: key: %1 not found").arg(QVariant::fromValue(key).toString()));
                return false;
            }

            if (table[idx].status == 1 && table[idx].key == key && table[idx].value == value) {
                table[idx].status = 0;

                for (int jdx = size - 1; jdx > idx; jdx--) {

                    if (table[jdx].status == 1 && hash(table[jdx].key) == h) {
                        table[idx].key = move(table[jdx].key);
                        table[idx].value = move(table[jdx].value);
                        table[idx].status = 1;
                        table[jdx].status = 0;
                        table[jdx].key = K{};
                        table[jdx].value = V{};
                    }
                }
                loger.enqueue(QString("HashTable remove: key: %1 value: %2 removed succsessfull").arg(QVariant::fromValue(key).toString()).arg(QVariant::fromValue(value).toString()));
                return true;
            }
        }
        loger.enqueue(QString("HashTable remove: key: %1 not found").arg(QVariant::fromValue(key).toString()));
        return false;
    }

    
    void print() const {
        for (int i = 0; i < size; ++i) {
            if (table[i].status == 1) {
                cout << " " << i << ":\n";
                table[i].value.print();
            }
        }
    }
};
