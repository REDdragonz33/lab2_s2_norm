#ifndef ARRAY_H
#define ARRAY_H

#include "istructure.h"
#include <iostream>
#include <string>
using namespace std;

class Array : public IStructure {
private:
    string* data;
    size_t size;
    size_t capacity;

    void resize(size_t new_cap);

public:
    Array();
    ~Array();

    void insertEnd(string val);
    void insertAt(int idx, string val);
    void removeAt(int idx);
    string get(int idx);
    void set(int idx, string val);
    int length();
    
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};

#endif