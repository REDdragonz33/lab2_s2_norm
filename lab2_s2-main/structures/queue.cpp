#include "queue.h"

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


// вставка
void Queue::insert(string val) {
    Node* newNode = new Node(val);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
}

// удаление
string Queue::remove() {
    if (head == nullptr) {
        return "EMPTY";
    }
    Node* temp = head;
    string val = temp->data;
    head = head->next;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete temp;
    return val;
}

void Queue::print() {
    Node* current = head;
    cout << "Queue (front -> back): ";
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

void Queue::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "QPUSH") {
        if (tokens.size() < 3) { cout << "QPUSH: missing value" << endl; return; }
        insert(tokens[2]);
    } else if (cmd == "QPOP") {
        cout << remove() << endl;
    }
}

char Queue::getType() const { return 'Q'; }

void Queue::save(ostream& out) const {
    Node* cur = head;
    int count = 0;
    while (cur) { count++; cur = cur->next; }
    out << count;
    cur = head;
    while (cur) { out << " " << cur->data; cur = cur->next; }
}

void Queue::load(istream& in) {
    while (head) { Node* t = head; head = head->next; delete t; }
    tail = nullptr;
    
    int count; 
    if (!(in >> count)) return;
    
    for (int i = 0; i < count; ++i) {
        string v; 
        if (!(in >> v)) break;
        insert(v);
    }
}