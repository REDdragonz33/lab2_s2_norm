#include "z6_v2_vv1_doublehash.h"
#include <iostream>
#include <chrono>

// конструктор
HashTable::HashTable(int cap) : cap(cap), chainCollisions(0), chainCount(0),
    cuckooCap(32), cuckooCount(0), cuckooCollisions(0) {
    buckets = new DList*[cap];
    for (int i = 0; i < cap; ++i) {
        buckets[i] = new DList();
    }
    cuckoo = new string[cuckooCap];
    for (int i = 0; i < cuckooCap; ++i) {
        cuckoo[i] = "";
    }
}

// деструктор
HashTable::~HashTable() {
    for (int i = 0; i < cap; ++i) {
        delete buckets[i];
    }
    delete[] buckets;
    delete[] cuckoo;
}

// хеш 1 (метод деления)
int HashTable::h1(const string& key, int size) const {
    int h = 0;
    for (char c : key) {
        h = (h * 31 + (int)c) % size;
    }
    return h < 0 ? h + size : h;
}

// хеш 2 (метод умножения)
int HashTable::h2(const string& key, int size) const {
    int h = 0;
    for (char c : key) {
        h = (h * 131 + (int)c) % size;
    }
    return h < 0 ? h + size : h;
}

// цепочки: вставка
void HashTable::chainPut(const string& key, const string& val) {
    int idx = h1(key, cap);
    DNode* old = buckets[idx]->findNodeByPrefix(key + ":");
    if (old) {
        buckets[idx]->removeNode(old);
    } else {
        if (buckets[idx]->getSize() > 0) chainCollisions++;
        chainCount++;
    }
    buckets[idx]->insertTail(key + ":" + val);
}

// цепочки: чтение
string HashTable::chainGet(const string& key) {
    int idx = h1(key, cap);
    DNode* n = buckets[idx]->findNodeByPrefix(key + ":");
    if (!n) {
        return "EMPTY";
    }
    return n->data.substr(key.length() + 1);
}

// цепочки: удаление
void HashTable::chainDel(const string& key) {
    int idx = h1(key, cap);
    DNode* n = buckets[idx]->findNodeByPrefix(key + ":");
    if (n) {
        buckets[idx]->removeNode(n);
        chainCount--;
    }
}

// кукушка: реинсертирование
void HashTable::cuckooRehash() {
    string* old = cuckoo;
    int oldCap = cuckooCap;
    cuckooCap *= 2;
    cuckoo = new string[cuckooCap];
    for (int i = 0; i < cuckooCap; ++i) {
        cuckoo[i] = "";
    }
    cuckooCount = 0;

    for (int i = 0; i < oldCap; ++i) {
        if (old[i].empty()) continue;
        size_t p = old[i].find(':');
        if (p == string::npos) continue;
        string k = old[i].substr(0, p);
        string v = old[i].substr(p + 1);
        cuckooInsert(k, v, 0);
    }
    delete[] old;
}

// кукушка: вставка с вытеснением
bool HashTable::cuckooInsert(const string& key, const string& val, int depth) {
    int i1 = h1(key, cuckooCap);
    int i2 = h2(key, cuckooCap);
    string pref = key + ":";

    if (cuckoo[i1].compare(0, pref.length(), pref) == 0) {
        cuckoo[i1] = key + ":" + val;
        return true;
    }
    if (cuckoo[i2].compare(0, pref.length(), pref) == 0) {
        cuckoo[i2] = key + ":" + val;
        return true;
    }

    string current = key + ":" + val;
    int pos = i1;
    int maxKicks = cuckooCap * 2;
    for (int kick = 0; kick < maxKicks; ++kick) {
        if (cuckoo[pos].empty()) {
            cuckoo[pos] = current;
            ++cuckooCount;
            return true;
        }

        ++cuckooCollisions;
        string evicted = cuckoo[pos];
        cuckoo[pos] = current;
        size_t p = evicted.find(':');
        string evictedKey = evicted.substr(0, p);
        current = evicted;
        int first = h1(evictedKey, cuckooCap);
        int second = h2(evictedKey, cuckooCap);
        pos = (pos == first) ? second : first;
    }

    // При цикле сохраняем вытесненный элемент, увеличиваем таблицу и вставляем его.
    size_t p = current.find(':');
    string pendingKey = current.substr(0, p);
    string pendingVal = current.substr(p + 1);
    cuckooRehash();
    if (depth > 8) cuckooRehash();
    return cuckooInsert(pendingKey, pendingVal, depth + 1);
}

// кукушка: публичная вставка
bool HashTable::cuckooPut(const string& key, const string& val) {
    return cuckooInsert(key, val, 0);
}

// кукушка: чтение
string HashTable::cuckooGet(const string& key) {
    int i1 = h1(key, cuckooCap);
    int i2 = h2(key, cuckooCap);
    string pref = key + ":";
    if (cuckoo[i1].substr(0, pref.length()) == pref) {
        return cuckoo[i1].substr(pref.length());
    }
    if (cuckoo[i2].substr(0, pref.length()) == pref) {
        return cuckoo[i2].substr(pref.length());
    }
    return "EMPTY";
}

// эмпирический анализ
void HashTable::analyze() {
    cout << "=== Chains ===" << endl;
    cout << "Count=" << chainCount << " Collisions=" << chainCollisions << endl;
    cout << "Method: separate chaining (each bucket stores a DList)." << endl;
    cout << "Average: O(1 + load factor), worst: O(n)." << endl;
    cout << "Pros: simple insertion/deletion, load factor can exceed 1." << endl;
    cout << "Cons: extra memory for nodes, linear search inside a bucket." << endl;
    cout << "=== Cuckoo ===" << endl;
    cout << "Count=" << cuckooCount << " Collisions=" << cuckooCollisions << endl;
    cout << "Method: cuckoo hashing (a key can occupy one of two hash positions)." << endl;
    cout << "Lookup: O(1), insertion: O(1) amortized, rehash: O(n)." << endl;
    cout << "Pros: at most two positions are checked during lookup." << endl;
    cout << "Cons: insertion can trigger rehashing; deletion needs tombstones or markers." << endl;

    // Небольшой эмпирический тест на одинаковом наборе ключей.
    const int testCount = 1000;
    HashTable chainTest(64);
    HashTable cuckooTest(64);
    auto startChain = chrono::steady_clock::now();
    for (int i = 0; i < testCount; ++i) {
        string key = "test_key_" + to_string(i);
        chainTest.chainPut(key, "value");
    }
    for (int i = 0; i < testCount; ++i) {
        chainTest.chainGet("test_key_" + to_string(i));
    }
    auto endChain = chrono::steady_clock::now();

    auto startCuckoo = chrono::steady_clock::now();
    for (int i = 0; i < testCount; ++i) {
        string key = "test_key_" + to_string(i);
        cuckooTest.cuckooPut(key, "value");
    }
    for (int i = 0; i < testCount; ++i) {
        cuckooTest.cuckooGet("test_key_" + to_string(i));
    }
    auto endCuckoo = chrono::steady_clock::now();

    long long chainTime = chrono::duration_cast<chrono::microseconds>(endChain - startChain).count();
    long long cuckooTime = chrono::duration_cast<chrono::microseconds>(endCuckoo - startCuckoo).count();
    cout << "=== Empirical test (" << testCount << " insertions + " << testCount << " lookups) ===" << endl;
    cout << "Chains: " << chainTime << " us, collisions=" << chainTest.chainCollisions << endl;
    cout << "Cuckoo: " << cuckooTime << " us, collisions=" << cuckooTest.cuckooCollisions << endl;
    cout << "Note: time depends on the computer and current system load." << endl;
}

// собрать рёбра с префиксом
void HashTable::collectEdges(const string& prefix, DList& out) {
    for (int i = 0; i < cap; ++i) {
        buckets[i]->collectByPrefix(prefix, out);
    }
}

// выводим
void HashTable::print() {
    cout << "HashTable ready" << endl;
}

// команди
void HashTable::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "HT_PUT") {
        if (tokens.size() < 3) return;
        chainPut(tokens[1], tokens[2]);
    } else if (cmd == "HT_GET") {
        if (tokens.size() < 2) return;
        cout << chainGet(tokens[1]) << endl;
    } else if (cmd == "HT_DEL") {
        if (tokens.size() < 2) return;
        chainDel(tokens[1]);
    } else if (cmd == "CK_PUT") {
        if (tokens.size() < 3) return;
        cuckooPut(tokens[1], tokens[2]);
    } else if (cmd == "CK_GET") {
        if (tokens.size() < 2) return;
        cout << cuckooGet(tokens[1]) << endl;
    } else if (cmd == "HT_ANALYZE") {
        analyze();
    }
}

// сохранялко
void HashTable::save(ostream& out) const {
    out << "HT " << chainCount;
}

// загрузко
void HashTable::load(istream& in) {
    string d;
    int n;
    in >> d >> n;
}

// тип
char HashTable::getType() const {
    return '6';
}