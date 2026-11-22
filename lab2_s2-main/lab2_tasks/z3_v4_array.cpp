#include "z3_v4_array.h"
#include <iostream>

// конструктор
MaxIntersect::MaxIntersect() : count(0), maxSets(16) {
    sets = new Set*[maxSets];
    for (int i = 0; i < maxSets; ++i) sets[i] = nullptr;
}

// деструктор
MaxIntersect::~MaxIntersect() {
    for (int i = 0; i < count; ++i) delete sets[i];
    delete[] sets;
}

// создаем новое множество
void MaxIntersect::newSet() {
    if (count == maxSets) {
        int newCap = maxSets * 2;
        Set** bigger = new Set*[newCap];
        for (int i = 0; i < count; ++i) bigger[i] = sets[i];
        for (int i = count; i < newCap; ++i) bigger[i] = nullptr;
        delete[] sets;
        sets = bigger;
        maxSets = newCap;
    }
    sets[count++] = new Set();
    cout << "Set " << count - 1 << " created" << endl;
}

// добавляем элемент
void MaxIntersect::addElem(int idx, const string& val) {
    if (idx < 0 || idx >= count) {
        cout << "bad set idx" << endl;
        return;
    }
    sets[idx]->add(val);
}

// выводим множество
void MaxIntersect::printSet(const Set* set) const {
    cout << "{";
    for (int i = 0; i < set->size(); ++i) {
        if (i) cout << ", ";
        cout << set->getAt(i);
    }
    cout << "}";
}

// ищем пару с макс пересечением
void MaxIntersect::findMax() {
    if (count < 2) {
        cout << "Need >= 2 sets" << endl;
        return;
    }

    int best = -1;
    int bi = -1;
    int bj = -1;

    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            Set currentCommon;
            Set* small = (sets[i]->size() <= sets[j]->size()) ? sets[i] : sets[j];
            Set* big = (sets[i]->size() <= sets[j]->size()) ? sets[j] : sets[i];

            for (int k = 0; k < small->size(); ++k) {
                string value = small->getAt(k);
                if (big->has(value)) currentCommon.add(value);
            }

            if (currentCommon.size() > best) {
                best = currentCommon.size();
                bi = i;
                bj = j;
            }
        }
    }

    cout << "Max common = " << best << endl;
    cout << "Sets: ";
    printSet(sets[bi]);
    cout << " and ";
    printSet(sets[bj]);
    cout << endl;
    Set commonSet;
    Set* small = (sets[bi]->size() <= sets[bj]->size()) ? sets[bi] : sets[bj];
    Set* big = (sets[bi]->size() <= sets[bj]->size()) ? sets[bj] : sets[bi];
    for (int i = 0; i < small->size(); ++i) {
        string value = small->getAt(i);
        if (big->has(value)) commonSet.add(value);
    }
    cout << "Common elements: ";
    printSet(&commonSet);
    cout << endl;
}

// выводим
void MaxIntersect::print() {
    cout << "MaxIntersect ready, sets=" << count << endl;
    for (int i = 0; i < count; ++i) {
        cout << i << ": ";
        printSet(sets[i]);
        cout << endl;
    }
}

// команди
void MaxIntersect::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "SETS_NEW") {
        newSet();
    } else if (cmd == "SETS_ADD") {
        if (tokens.size() < 3) {
            cout << "SETS_ADD: missing args" << endl;
            return;
        }
        addElem(stoi(tokens[1]), tokens[2]);
    } else if (cmd == "SETS_MAX") {
        findMax();
    } else if (cmd == "SETS_PRINT") {
        print();
    }
}

// сохранялко
void MaxIntersect::save(ostream& out) const {
    out << count << '\n';
    for (int i = 0; i < count; ++i) {
        sets[i]->save(out);
        out << '\n';
    }
}

// загрузко
void MaxIntersect::load(istream& in) {
    for (int i = 0; i < count; ++i) delete sets[i];
    count = 0;
    int n;
    if (!(in >> n)) return;
    for (int i = 0; i < n; ++i) {
        newSet();
        sets[count - 1]->load(in);
    }
}

// тип
char MaxIntersect::getType() const {
    return '3';
}