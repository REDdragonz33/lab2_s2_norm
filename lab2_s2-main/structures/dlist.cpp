#include "dlist.h"

DNode::DNode(string val, DNode* p, DNode* n) : data(val), prev(p), next(n) {}

DList::DList() : head(nullptr), tail(nullptr) {}

DList::~DList() {
    while (head != nullptr) {
        DNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// вставка наверх
void DList::insertHead(string val) {
    DNode* newNode = new DNode(val, nullptr, head);
    if (head != nullptr) {
        head->prev = newNode;
    } else {
        tail = newNode;
    }
    head = newNode;
}

// вставка в хвост
void DList::insertTail(string val) {
    DNode* newNode = new DNode(val, tail, nullptr);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
}

// вставка после эла
void DList::insertAfter(string target, string val) {
    DNode* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }
    if (current != nullptr) {
        DNode* newNode = new DNode(val, current, current->next);
        if (current->next != nullptr) {
            current->next->prev = newNode;
        } else {
            tail = newNode;
        }
        current->next = newNode;
    }
}

// вставка перед элом
void DList::insertBefore(string target, string val) {
    DNode* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }
    if (current != nullptr) {
        DNode* newNode = new DNode(val, current->prev, current);
        if (current->prev != nullptr) {
            current->prev->next = newNode;
        } else {
            head = newNode;
        }
        current->prev = newNode;
    }
}


// удаление по значению
void DList::remove(string val) {
    DNode* current = head;
    while (current != nullptr && current->data != val) {
        current = current->next;
    }
    if (current != nullptr) {
        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        delete current;
    }
}

void DList::removeAfter(string target) {
    if (head == nullptr) return;

    DNode* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current != nullptr && current->next != nullptr) {
        DNode* temp = current->next;
        current->next = temp->next;
        
        if (temp->next != nullptr) {
            temp->next->prev = current;
        } else {
            tail = current;
        }
        delete temp;
    }
}

void DList::removeBefore(string target) {
    if (head == nullptr) return;

    DNode* current = head;
    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current != nullptr && current->prev != nullptr) {
        DNode* temp = current->prev;
        current->prev = temp->prev;
        
        if (temp->prev != nullptr) {
            temp->prev->next = current;
        } else {
            head = current;
        }
        delete temp;
    }
}

void DList::print() {
    DNode* current = head;
    cout << "DList: ";
    while (current != nullptr) {
        cout << current->data << " <-> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}

void DList::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "LPUSH") {
        if (tokens.size() < 4) { cout << "LPUSH: missing args" << endl; return; }
        string mode = tokens[2];
        string val = tokens[3];
        if (mode == "HEAD") insertHead(val);
        else if (mode == "TAIL") insertTail(val);
        else if (mode == "AFTER" && tokens.size() >= 5) insertAfter(val, tokens[4]);
        else if (mode == "BEFORE" && tokens.size() >= 5) insertBefore(val, tokens[4]);
    }   else if (cmd == "LDEL") {
        if (tokens.size() < 3) { cout << "LDEL: missing value" << endl; return; }
        
        if (tokens.size() >= 4) {
            string mode = tokens[2];
            string val = tokens[3];
            if (mode == "AFTER") removeAfter(val);
            else if (mode == "BEFORE") removeBefore(val);
            else remove(val); 
        } else {
            remove(tokens[2]);
        }
    }
}

char DList::getType() const { return 'L'; }

void DList::save(ostream& out) const {
    DNode* cur = head;
    int count = 0;
    while (cur) { count++; cur = cur->next; }
    out << count;
    cur = head;
    while (cur) { out << " " << cur->data; cur = cur->next; }
}

void DList::load(istream& in) {
    while (head) { DNode* t = head; head = head->next; delete t; }
    tail = nullptr;
    
    int count; 
    if (!(in >> count)) return;
    
    for (int i = 0; i < count; ++i) {
        string v; 
        if (!(in >> v)) break;
        insertTail(v);
    }
}

// есть ли такой эл
bool DList::contains(const string& val) {
    DNode* cur = head;
    while (cur != nullptr) {
        if (cur->data == val) return true;
        cur = cur->next;
    }
    return false;
}

// удалялко с хвоста
string DList::removeTail() {
    if (tail == nullptr) return "EMPTY";
    DNode* temp = tail;
    string val = temp->data;
    tail = temp->prev;
    if (tail != nullptr) tail->next = nullptr;
    else head = nullptr;
    delete temp;
    return val;
}

// узнаем размер
int DList::getSize() const {
    int c = 0;
    DNode* cur = head;
    while (cur) { c++; cur = cur->next; }
    return c;
}

// очистка
void DList::clear() {
    while (head) { DNode* t = head; head = head->next; delete t; }
    tail = nullptr;
}

// ищем узел по префиксу (для LFU)
DNode* DList::findNodeByPrefix(const string& prefix) {
    DNode* cur = head;
    while (cur != nullptr) {
        if (cur->data.length() >= prefix.length() &&
            cur->data.substr(0, prefix.length()) == prefix) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// удаляем конкретный узел
void DList::removeNode(DNode* node) {
    if (node == nullptr) return;
    if (node->prev != nullptr) node->prev->next = node->next;
    else head = node->next;
    if (node->next != nullptr) node->next->prev = node->prev;
    else tail = node->prev;
    delete node;
}

// собираем все узлы с префиксом в другой список (для маршрутов)
void DList::collectByPrefix(const string& prefix, DList& out) {
    DNode* cur = head;
    while (cur != nullptr) {
        if (cur->data.length() >= prefix.length() &&
            cur->data.substr(0, prefix.length()) == prefix)
            out.insertTail(cur->data);
        cur = cur->next;
    }
}

DNode* DList::getHead() const {
    return head;
}