#ifndef Z6_V4_VV2_OPTI_H
#define Z6_V4_VV2_OPTI_H
#include "../structures/istructure.h"
#include "z6_v2_vv1_doublehash.h"   // было: doubhesh.h
#include <string>
using namespace std;
class RouteOptimizer : public IStructure {
private:
    HashTable ht;
public:
    RouteOptimizer(); ~RouteOptimizer();
    void addRoute(const string& a,const string& b,long long delay);
    long long dijkstra(const string& src,const string& dst);
    void print() override;
    void execute(const string& cmd,const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif
