#ifndef ISTRUCTURE_H
#define ISTRUCTURE_H

#include <iostream>
#include <string>
using namespace std;

class TokenList {
private:
    string* items;
    int count;
    int capacity;
    void grow() {
        int next = capacity * 2;
        string* data = new string[next];
        for (int i = 0; i < count; ++i) data[i] = items[i];
        delete[] items; items = data; capacity = next;
    }
public:
    TokenList() : items(new string[8]), count(0), capacity(8) {}
    TokenList(const TokenList& other) : items(new string[other.capacity]), count(other.count), capacity(other.capacity) {
        for (int i = 0; i < count; ++i) items[i] = other.items[i];
    }
    TokenList& operator=(const TokenList& other) {
        if (this == &other) return *this;
        delete[] items; capacity = other.capacity; count = other.count; items = new string[capacity];
        for (int i = 0; i < count; ++i) items[i] = other.items[i];
        return *this;
    }
    ~TokenList() { delete[] items; }
    void push_back(const string& value) { if (count == capacity) grow(); items[count++] = value; }
    int size() const { return count; }
    bool empty() const { return count == 0; }
    const string& operator[](int index) const { return items[index]; }
    string& operator[](int index) { return items[index]; }
};

class IStructure {
public:
    virtual ~IStructure() {}
    virtual void execute(const string& cmd, const TokenList& tokens) = 0;
    virtual void print() = 0;
    virtual void save(ostream& out) const = 0;
    virtual void load(istream& in) = 0;
    virtual char getType() const = 0;
};

#endif