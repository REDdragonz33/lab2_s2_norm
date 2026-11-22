#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

struct Node {
    string data;
    Node* next;
    Node(string val, Node* n = nullptr) : data(val), next(n) {}
};

#endif