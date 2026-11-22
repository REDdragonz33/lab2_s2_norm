#ifndef Z7_V2_LFU_H
#define Z7_V2_LFU_H
#include "../structures/istructure.h"
#include "../structures/dlist.h"
#include <string>
using namespace std;

class LFUCache : public IStructure {
private:
    DList list; // узлы вида "key|freq|value"
    int capacity;
    void bump(DNode* node);
    void evictMin();
public:
    LFUCache(int cap);
    ~LFUCache();
    void set(const string& key, const string& val);
    string get(const string& key);
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif