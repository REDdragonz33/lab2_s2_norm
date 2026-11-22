#include "z4_v4_piram.h"
#include <iostream>
#include <sstream>

// конструктор
Pyramid::Pyramid() {}

// деструктор
Pyramid::~Pyramid() {}

// получаем блок из массива ЛР1
Block Pyramid::readBlock(int index) const {
    string value = const_cast<Array&>(blocks).get(index);
    size_t p = value.find(',');
    if (p == string::npos) return {0, 0};
    return {stoi(value.substr(0, p)), stoi(value.substr(p + 1))};
}

// вставка блока
void Pyramid::addBlock(int w, int h) {
    if (w <= 0 || h <= 0) {
        cout << "Block dimensions must be positive" << endl;
        return;
    }
    blocks.insertEnd(to_string(w) + "," + to_string(h));
}

// сортировка слиянием
void Pyramid::mergeSort(Block* a, Block* temp, int left, int right) {
    if (right - left <= 1) return;
    int m = left + (right - left) / 2;
    mergeSort(a, temp, left, m);
    mergeSort(a, temp, m, right);
    int i = left, j = m, k = left;
    while (i < m && j < right) {
        if (a[i].w >= a[j].w) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i < m) temp[k++] = a[i++];
    while (j < right) temp[k++] = a[j++];
    for (i = left; i < right; ++i) a[i] = temp[i];
}

// сортировка по убыванию ширины
void Pyramid::sortDesc() {
    int count = const_cast<Array&>(blocks).length();
    if (count < 2) return;
    Block* data = new Block[count];
    Block* temp = new Block[count];
    for (int i = 0; i < count; ++i) data[i] = readBlock(i);
    mergeSort(data, temp, 0, count);
    for (int i = 0; i < count; ++i) {
        blocks.set(i, to_string(data[i].w) + "," + to_string(data[i].h));
    }
    delete[] data;
    delete[] temp;
}

// вычисляем макс высоту
long long Pyramid::calcMax() {
    int count = const_cast<Array&>(blocks).length();
    if (!count) return 0;
    sortDesc();
    long long total = 0;
    int i = 0;
    while (i < count) {
        Block current = readBlock(i);
        int width = current.w;
        int bestH = current.h;
        int j = i + 1;
        while (j < count) {
            Block next = readBlock(j);
            if (next.w != width) break;
            if (next.h > bestH) bestH = next.h;
            ++j;
        }
        total += bestH;
        i = j;
    }
    return total;
}

// выводим
void Pyramid::print() {
    cout << "Pyramid ready, blocks=" << blocks.length() << endl;
}

// команди
void Pyramid::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "PIRAM_ADD") {
        if (tokens.size() < 3) {
            cout << "PIRAM_ADD: missing args" << endl;
            return;
        }
        addBlock(stoi(tokens[1]), stoi(tokens[2]));
    } else if (cmd == "PIRAM_CALC") {
        cout << "Max height: " << calcMax() << endl;
    } else if (cmd == "PIRAM_CLEAR") {
        while (blocks.length() > 0) {
            blocks.removeAt(blocks.length() - 1);
        }
    }
}

// сохранялко
void Pyramid::save(ostream& out) const {
    int count = const_cast<Array&>(blocks).length();
    out << count;
    for (int i = 0; i < count; ++i) {
        Block b = readBlock(i);
        out << ' ' << b.w << ' ' << b.h;
    }
}

// загрузко
void Pyramid::load(istream& in) {
    while (blocks.length() > 0) {
        blocks.removeAt(blocks.length() - 1);
    }
    int n;
    if (!(in >> n)) return;
    for (int i = 0; i < n; ++i) {
        int w, h;
        if (!(in >> w >> h)) break;
        addBlock(w, h);
    }
}

// тип
char Pyramid::getType() const {
    return '4';
}