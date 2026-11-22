#ifndef SLIST_H
#define SLIST_H

#include "node.h"
#include "istructure.h"
#include <iostream>
using namespace std;

class SList : public IStructure {
private:
    Node* head;
    Node* tail;

public:
    SList();
    ~SList();

    void insertHead(string val);
    void insertTail(string val);
    void insertAfter(string target, string val);
    void insertBefore(string target, string val);
    void remove(string val);
    
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
    void removeAfter(string target);
    void removeBefore(string target);
};

#endif