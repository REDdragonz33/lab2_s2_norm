#ifndef Z5_V4_TREE_H
#define Z5_V4_TREE_H
#include "../structures/istructure.h"
#include "../structures/tree.h"
#include <string>
using namespace std;
class TreeForks : public IStructure {
private:
    TNode* root;
    void insert(int val);
    void destroy(TNode* n);
public:
    TreeForks(); ~TreeForks();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif
