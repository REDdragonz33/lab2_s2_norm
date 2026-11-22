#include "z5_v4_tree.h"
#include <iostream>

// конструктор
TreeForks::TreeForks() : root(nullptr) {}

// деструктор
TreeForks::~TreeForks() {
    destroy(root);
}

// уничтожалко
void TreeForks::destroy(TNode* n) {
    if (!n) return;
    int cap = 16;
    int top = 0;
    TNode** stack = new TNode*[cap];
    stack[top++] = n;
    while (top) {
        TNode* cur = stack[--top];
        if (cur->left) {
            if (top == cap) {
                TNode** bigger = new TNode*[cap * 2];
                for (int i = 0; i < top; ++i) {
                    bigger[i] = stack[i];
                }
                delete[] stack;
                stack = bigger;
                cap *= 2;
            }
            stack[top++] = cur->left;
        }
        if (cur->right) {
            if (top == cap) {
                TNode** bigger = new TNode*[cap * 2];
                for (int i = 0; i < top; ++i) {
                    bigger[i] = stack[i];
                }
                delete[] stack;
                stack = bigger;
                cap *= 2;
            }
            stack[top++] = cur->right;
        }
        delete cur;
    }
    delete[] stack;
}

// вставка
void TreeForks::insert(int val) {
    if (!root) {
        root = new TNode(val);
        return;
    }
    TNode* cur = root;
    while (true) {
        if (val < cur->data) {
            if (!cur->left) {
                cur->left = new TNode(val);
                return;
            }
            cur = cur->left;
        } else if (val > cur->data) {
            if (!cur->right) {
                cur->right = new TNode(val);
                return;
            }
            cur = cur->right;
        } else {
            return;
        }
    }
}

// выводим
void TreeForks::print() {
    cout << "TreeForks ready" << endl;
}

// команди
void TreeForks::execute(const string& cmd, const TokenList& tokens) {
    if (cmd != "TREE_FORKS") return;
    destroy(root);
    root = nullptr;
    for (int i = 1; i < tokens.size(); ++i) {
        int v = stoi(tokens[i]);
        if (v == 0) break;
        insert(v);
    }

    // Собственный динамический стек указателей; inorder сразу даёт возрастающий порядок BST.
    int stackCap = 16;
    int top = 0;
    int resultCap = 16;
    int resultCount = 0;
    TNode** stack = new TNode*[stackCap];
    int* result = new int[resultCap];
    TNode* cur = root;
    while (cur || top) {
        while (cur) {
            if (top == stackCap) {
                TNode** bigger = new TNode*[stackCap * 2];
                for (int i = 0; i < top; ++i) {
                    bigger[i] = stack[i];
                }
                delete[] stack;
                stack = bigger;
                stackCap *= 2;
            }
            stack[top++] = cur;
            cur = cur->left;
        }
        cur = stack[--top];
        if (cur->left && cur->right) {
            if (resultCount == resultCap) {
                int* bigger = new int[resultCap * 2];
                for (int i = 0; i < resultCount; ++i) {
                    bigger[i] = result[i];
                }
                delete[] result;
                result = bigger;
                resultCap *= 2;
            }
            result[resultCount++] = cur->data;
        }
        cur = cur->right;
    }
    cout << "Forks: ";
    for (int i = 0; i < resultCount; ++i) {
        cout << result[i] << ' ';
    }
    cout << endl;
    delete[] stack;
    delete[] result;
}

// сохранялко
void TreeForks::save(ostream& out) const {
    out << "FORKS";
}

// загрузко
void TreeForks::load(istream& in) {
    string d;
    in >> d;
}

// тип
char TreeForks::getType() const {
    return '5';
}