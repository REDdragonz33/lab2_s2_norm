#ifndef Z1_V2_STACK_H
#define Z1_V2_STACK_H
#include "../structures/istructure.h"
#include "../structures/stack.h"
#include <string>
using namespace std;

class LogicEval : public IStructure {
private:
    Stack vals;
    Stack ops;
    int prec(char op);
    void applyOp();
public:
    LogicEval();
    ~LogicEval();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif