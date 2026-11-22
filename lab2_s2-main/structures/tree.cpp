#include "tree.h"

void CompleteTree::destroyTree(TNode* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

CompleteTree::CompleteTree() : root(nullptr), nodeCount(0) {}

CompleteTree::~CompleteTree() {
    destroyTree(root);
}

char CompleteTree::getType() const { return 'T'; }

//вставка по узлам
void CompleteTree::insert(int val) {
    TNode* newNode = new TNode(val);

    if (root == nullptr) {
        root = newNode;
        nodeCount++;
        cout << "Inserted " << val << endl;
        return;
    }

    nodeCount++;
    int target = nodeCount;
    int bits[32];
    int bitCount = 0;
    while (target > 1) {
        bits[bitCount++] = target % 2;
        target /= 2;
    }

    TNode* current = root;
    for (int i = bitCount - 1; i >= 0; --i) {
        if (i == 0) {
            if (bits[i] == 0) current->left = newNode;
            else current->right = newNode;
        } else {
            if (bits[i] == 0) current = current->left;
            else current = current->right;
        }
    }
    cout << "Inserted " << val << endl;
}

//поиск
static bool searchRec(TNode* node, int val) {
    if (node == nullptr) return false;
    if (node->data == val) return true;
    return searchRec(node->left, val) || searchRec(node->right, val);
}

bool CompleteTree::search(int val) {
    return searchRec(root, val);
}

//проверко
bool CompleteTree::isCompleteRec(TNode* node, int index, int totalNodes) {
    if (node == nullptr) return true;
    if (index >= totalNodes) return false;
    
    return isCompleteRec(node->left, 2 * index + 1, totalNodes) &&
           isCompleteRec(node->right, 2 * index + 2, totalNodes);
}

void CompleteTree::check() {
    cout << (isCompleteRec(root, 0, nodeCount) ? "TRUE" : "FALSE") << endl;
}

//выводим
void CompleteTree::print() {
    cout << "Tree (levels): ";
    if (!root) { cout << endl; return; }

    Queue q; 
    q.insert(to_string((long long)root));

    while (true) {
        string ptrStr = q.remove();
        if (ptrStr == "EMPTY") break;
        
        TNode* cur = (TNode*)stoll(ptrStr);
        cout << cur->data << " ";
        
        if (cur->left) q.insert(to_string((long long)cur->left));
        if (cur->right) q.insert(to_string((long long)cur->right));
    }
    cout << endl;
}

//сохранялко
void CompleteTree::save(ostream& out) const {
    out << nodeCount;
    if (!root) return;

    Queue q;
    q.insert(to_string((long long)root));

    while (true) {
        string ptrStr = q.remove();
        if (ptrStr == "EMPTY") break;
        
        TNode* cur = (TNode*)stoll(ptrStr);
        out << " " << cur->data;
        
        if (cur->left) q.insert(to_string((long long)cur->left));
        if (cur->right) q.insert(to_string((long long)cur->right));
    }
}

//загрузко
void CompleteTree::load(istream& in) {
    destroyTree(root);
    root = nullptr;
    nodeCount = 0;
    
    int count;
    if (!(in >> count)) return;
    
    for (int i = 0; i < count; ++i) {
        int val;
        if (!(in >> val)) break;
        insert(val); 
    }
}

//команди
void CompleteTree::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "TINSERT") {
        if (tokens.size() < 3) { cout << "TINSERT: missing value" << endl; return; }
        insert(stoi(tokens[2]));
    } else if (cmd == "TGET") {
        if (tokens.size() < 3) { cout << "TGET: missing value" << endl; return; }
        cout << (search(stoi(tokens[2])) ? "TRUE" : "FALSE") << endl;
    } else if (cmd == "TCHECK") {
        check();
    }
}