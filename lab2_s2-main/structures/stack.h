#ifndef STACK_H
#define STACK_H
#include "istructure.h"
#include "node.h"
#include <iostream>
using namespace std;


class Stack : public IStructure{
private:
    Node* head; 

public:

    Stack();
    ~Stack();
    void insert(string val);
    string remove();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override; 
};

#endif