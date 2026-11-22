#include "slist.h"

SList::SList() : head(nullptr), tail(nullptr) {}

SList::~SList() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


// вставка наверх
void SList::insertHead(string val) {
    Node* newNode = new Node(val, head);
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
}


// вставка назад
void SList::insertTail(string val) {
    Node* newNode = new Node(val);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
}


// вставка после эла
void SList::insertAfter(string target, string val) {
    Node* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }
    if (current != nullptr) {
        Node* newNode = new Node(val, current->next);
        current->next = newNode;
        if (current == tail) {
            tail = newNode;
        }
    }
}


// вставка перед элом
void SList::insertBefore(string target, string val) {
    if (head == nullptr) return;
    
    if (head->data == target) {
        insertHead(val);
        return;
    }
    
    Node* current = head;
    while (current->next != nullptr && current->next->data != target) {
        current = current->next;
    }
    if (current->next != nullptr) {
        Node* newNode = new Node(val, current->next);
        current->next = newNode;
    }
}


// удаление по зн
void SList::remove(string val) {
    if (head == nullptr) return;

    if (head->data == val) {
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        return;
    }

    Node* current = head;
    while (current->next != nullptr && current->next->data != val) {
        current = current->next;
    }

    if (current->next != nullptr) {
        Node* temp = current->next;
        current->next = temp->next;
        if (temp == tail) {
            tail = current;
        }
        delete temp;
    }
}

void SList::print() {
    Node* current = head;
    cout << "SList: ";
    while (current != nullptr) {
        cout << current->data << " -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}

void SList::removeAfter(string target) {
    if (head == nullptr) return;

    Node* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current != nullptr && current->next != nullptr) {
        Node* temp = current->next;
        current->next = temp->next;
        
        if (temp == tail) {
            tail = current;
        }
        delete temp;
    }
}

void SList::removeBefore(string target) {
    if (head == nullptr || head->next == nullptr) return;

    if (head->next->data == target) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Node* current = head;
    while (current->next != nullptr && current->next->next != nullptr) {
        if (current->next->next->data == target) {
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            return;
        }
        current = current->next;
    }
}

void SList::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "FPUSH") {
        if (tokens.size() < 4) { cout << "FPUSH: missing args" << endl; return; }
        string mode = tokens[2];
        string val = tokens[3];
        if (mode == "HEAD") insertHead(val);
        else if (mode == "TAIL") insertTail(val);
        else if (mode == "AFTER" && tokens.size() >= 5) insertAfter(val, tokens[4]);
        else if (mode == "BEFORE" && tokens.size() >= 5) insertBefore(val, tokens[4]);
    } 
    else if (cmd == "FDEL") {
        if (tokens.size() < 3) { cout << "FDEL: missing value" << endl; return; }
        
        if (tokens.size() >= 4) {
            string mode = tokens[2];
            string target = tokens[3];
            
            if (mode == "AFTER") {
                removeAfter(target);
            } else if (mode == "BEFORE") {
                removeBefore(target);
            } else {
                remove(tokens[2]);
            }
        } else {
            remove(tokens[2]);
        }
    } 
    else if (cmd == "FGET") {
        cout << "FGET by index: not implemented" << endl;
    }
}
char SList::getType() const { return 'F'; }

void SList::save(ostream& out) const {
    Node* cur = head;
    int count = 0;
    while (cur) { count++; cur = cur->next; }
    out << count;
    cur = head;
    while (cur) { out << " " << cur->data; cur = cur->next; }
}

void SList::load(istream& in) {
    while (head) { Node* t = head; head = head->next; delete t; }
    tail = nullptr;
    
    int count; 
    if (!(in >> count)) return;
    
    for (int i = 0; i < count; ++i) {
        string v; 
        if (!(in >> v)) break;
        insertTail(v); 
    }
}

