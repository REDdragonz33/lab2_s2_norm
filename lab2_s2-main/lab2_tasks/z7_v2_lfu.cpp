#include "z7_v2_lfu.h"
#include <iostream>
#include <climits>

// конструктор
LFUCache::LFUCache(int cap) : capacity(cap) {}

// деструктор
LFUCache::~LFUCache() {}

// увеличить частоту узла
void LFUCache::bump(DNode* node) {
    string s = node->data;
    size_t p1 = s.find('|');
    size_t p2 = s.find('|', p1 + 1);
    
    string key = s.substr(0, p1);
    int f = stoi(s.substr(p1 + 1, p2 - p1 - 1));
    string val = s.substr(p2 + 1);
    
    if (f < INT_MAX) ++f;
    list.removeNode(node);
    list.insertTail(key + "|" + to_string(f) + "|" + val);
}

// вытеснить элемент с мин частотой
void LFUCache::evictMin() {
    DNode* cur = list.getHead();
    DNode* minNode = nullptr;
    int minF = INT_MAX;

    while (cur) {
        string s = cur->data;
        size_t p1 = s.find('|');
        size_t p2 = s.find('|', p1 + 1);
        int f = stoi(s.substr(p1 + 1, p2 - p1 - 1));
        
        if (f < minF) {
            minF = f;
            minNode = cur;
        }
        cur = cur->next;
    }

    if (minNode) {
        list.removeNode(minNode);
    }
}

// SET
void LFUCache::set(const string& key, const string& val) {
    if (capacity <= 0) return;
    
    DNode* n = list.findNodeByPrefix(key + "|");
    if (n) {
        string s = n->data;
        size_t p1 = s.find('|');
        size_t p2 = s.find('|', p1 + 1);
        int f = stoi(s.substr(p1 + 1, p2 - p1 - 1));
        
        list.removeNode(n);
        list.insertTail(key + "|" + to_string(f) + "|" + val);
    } else {
        if (list.getSize() >= capacity) {
            evictMin();
        }
        list.insertTail(key + "|1|" + val);
    }
}

// GET
string LFUCache::get(const string& key) {
    DNode* n = list.findNodeByPrefix(key + "|");
    if (!n) return "-1";
    
    string s = n->data;
    size_t p1 = s.find('|');
    size_t p2 = s.find('|', p1 + 1);
    string val = s.substr(p2 + 1);
    
    bump(n);
    return val;
}

// выводим
void LFUCache::print() {
    list.print();
}

// команди
void LFUCache::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "LFU_SET") {
        if (tokens.size() < 3) {
            cout << "LFU_SET: missing args" << endl;
            return;
        }
        set(tokens[1], tokens[2]);
    } 
    else if (cmd == "LFU_GET") {
        if (tokens.size() < 2) {
            cout << "LFU_GET: missing key" << endl;
            return;
        }
        cout << get(tokens[1]) << endl;
    } 
    else if (cmd == "LFU_CAP") {
        if (tokens.size() < 2) {
            cout << "LFU_CAP: missing capacity" << endl;
            return;
        }
        capacity = stoi(tokens[1]);
        if (capacity < 0) capacity = 0;
        
        while (list.getSize() > capacity) {
            evictMin();
        }
    }
}

// сохранялко
void LFUCache::save(ostream& out) const {
    out << capacity << " ";
    list.save(out);
}

// загрузко
void LFUCache::load(istream& in) {
    list.clear();
    if (!(in >> capacity)) return;
    if (capacity < 0) capacity = 0;
    list.load(in);
    while (list.getSize() > capacity) {
        evictMin();
    }
}

// тип
char LFUCache::getType() const {
    return '7';
}