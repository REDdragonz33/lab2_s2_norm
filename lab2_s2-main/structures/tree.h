#ifndef TREE_H
#define TREE_H

#include "istructure.h"
#include "queue.h" 
#include <iostream>
#include <string>
using namespace std;

struct TNode {
    int data;
    TNode* left;
    TNode* right;
    TNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

class CompleteTree : public IStructure {
private:
    TNode* root;
    int nodeCount; 
    void destroyTree(TNode* node);
    bool isCompleteRec(TNode* node, int index, int totalNodes);

public:
    CompleteTree();
    ~CompleteTree();

    void insert(int val);
    bool search(int val);
    void check();
    
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};

#endif