#include "stack.h"


// конструктор
Stack::Stack() : head(nullptr) {}


// деструктор
Stack::~Stack() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// вставка
void Stack::insert(string val) {
    Node* newNode = new Node(val, head);
    head = newNode;
}

// удаление
string Stack::remove() {
    if (head == nullptr) {
        return "EMPTY";
    }
    Node* temp = head;
    string val = temp->data;
    head = head->next;
    delete temp;
    return val;
}

// чтение
void Stack::print() {
    Node* current = head;
    cout << "Stack (top -> bottom): ";
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

void Stack::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "SPUSH") {
        if (tokens.size() < 3) { cout << "SPUSH: missing value" << endl; return; }
        insert(tokens[2]);
    } else if (cmd == "SPOP") {
        cout << remove() << endl;
    }
}

void Stack::save(ostream& out) const {
    Node* cur = head;
    int count = 0;
    while (cur) { count++; cur = cur->next; }
    out << count;
    cur = head;
    while (cur) { out << " " << cur->data; cur = cur->next; }
}

void Stack::load(istream& in) {
    while (head) { Node* t = head; head = head->next; delete t; }
    
    int count; 
    if (!(in >> count)) return;
    
    TokenList vals;
    for (int i = 0; i < count; ++i) {
        string v; 
        if (!(in >> v)) break;
        vals.push_back(v);
    }
    for (int i = vals.size() - 1; i >= 0; --i) {
        head = new Node(vals[i], head);
    }
}

char Stack::getType() const { 
    return 'S'; 
}