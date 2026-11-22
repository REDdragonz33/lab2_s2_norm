#ifndef Z6_V2_VV1_DOUBLEHASH_H
#define Z6_V2_VV1_DOUBLEHASH_H
#include "../structures/istructure.h"
#include "../structures/dlist.h"
#include <string>
using namespace std;

class HashTable : public IStructure {
private:
    DList** buckets;
    int cap;
    int chainCollisions;
    int chainCount;
    string* cuckoo;
    int cuckooCap;
    int cuckooCount;
    int cuckooCollisions;
    int h1(const string& key, int size) const;
    int h2(const string& key, int size) const;
    bool cuckooInsert(const string& key, const string& val, int depth);
    void cuckooRehash();
public:
    HashTable(int cap = 16);
    ~HashTable();
    void chainPut(const string& key, const string& val);
    string chainGet(const string& key);
    void chainDel(const string& key);
    bool cuckooPut(const string& key, const string& val);
    string cuckooGet(const string& key);
    void analyze();
    void collectEdges(const string& prefix, DList& out);
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif