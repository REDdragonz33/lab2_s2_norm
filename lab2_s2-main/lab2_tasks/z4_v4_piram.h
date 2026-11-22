#ifndef Z4_V4_PIRAM_H
#define Z4_V4_PIRAM_H
#include "../structures/istructure.h"
#include "../structures/array.h"
#include <string>
using namespace std;

struct Block { int w; int h; };

class Pyramid : public IStructure {
private:
    Array blocks; // массив из лабораторной работы №1
    Block readBlock(int index) const;
    void sortDesc();
    void mergeSort(Block* a, Block* temp, int left, int right);
public:
    Pyramid();
    ~Pyramid();
    void addBlock(int w, int h);
    long long calcMax();
    void print() override;
    void execute(const string& cmd, const TokenList& tokens) override;
    void save(ostream& out) const override;
    void load(istream& in) override;
    char getType() const override;
};
#endif
