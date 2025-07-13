#pragma once
#include <iostream> 
#include <string>
#include <algorithm>
#include <type_traits>
#include <QString>

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
        if constexpr (is_same_v<K, int>) {
            string temp_key = to_string(key);
            for (char c : key) h = h * 31 + c;
        }
        else if constexpr (is_same_v<K, QString>) {
            string temp = key.toStdString();
            for (char c : temp) h = h * 31 + c;
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
    HashTable(unsigned int sz) : size(sz) {

        table = new Entry[size];

        for (unsigned int i = 0; i < size; ++i) {
            table[i].status = 0;
        }
    }

    ~HashTable() {
        delete[] table;
    }

    bool insert(const V& value, const K& key) {
        unsigned int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                table[idx] = {key, value, 1 };
                return true;
            }

            if (table[idx].key == key) {
                return false;
            }
        }
        return false;
    }

    bool search(K& key, V* found_value = nullptr, int* steps= nullptr) const {
        unsigned int h = hash(key);
        if (steps) *steps = 0;
         if (found_value) *found_value = -1;
        for (int i = 0; i < size; ++i) {
            if (steps)*steps++;
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return false;
            if (table[idx].status == 1 && table[idx].key == key) {
                if (found_value) *found_value = table[idx].value;
                return true;
            }
        }
        return false;
    }

    bool remove(K& key, V&value) {
        unsigned int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return false;

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
                return true;
            }
        }
        return false;
    }

    
    void print() const {
        for (int i = 0; i < size; ++i) {
            if (table[i].status == 1) {
                cout << "������ " << i << ":\n";
                table[i].value.print();
            }
        }
    }
};
