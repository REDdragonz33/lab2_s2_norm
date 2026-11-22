#ifndef Z3_V4_ARRAY_H
#define Z3_V4_ARRAY_H
#include "../structures/istructure.h"
#include "z2_atd.h"
#include <string>
using namespace std;

class MaxIntersect : public IStructure {
private:
    Set** sets;
    int count;
    int maxSets;
    void printSet(const Set* set) const;
public:
    MaxIntersect();
    ~MaxIntersect();
    void newSet();
    void addElem(int idx, const string& val);
    void findMax();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif
