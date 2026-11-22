#ifndef Z2_ATD_H
#define Z2_ATD_H
#include "../structures/istructure.h"
#include <string>
using namespace std;

struct SetEntry {
    string value;
    bool occupied;
    bool deleted;
    SetEntry() : value(""), occupied(false), deleted(false) {}
};

class Set : public IStructure {
private:
    SetEntry* table;
    int capacity;
    int cnt;
    int hashFunc(const string& val) const;
    void rehash();
public:
    Set(int cap = 16);
    ~Set();
    void add(const string& val);
    void del(const string& val);
    bool has(const string& val) const;
    int size() const;
    string getAt(int index) const;
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif
