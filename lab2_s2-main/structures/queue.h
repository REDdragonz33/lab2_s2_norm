#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"
#include "istructure.h"
#include <iostream>
using namespace std;

class Queue : public IStructure {
private:
    Node* head;
    Node* tail;

public:
    Queue();
    ~Queue();
    void insert(string val);
    string remove();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};

#endif