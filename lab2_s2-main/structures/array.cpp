#include "array.h"

Array::Array() : data(nullptr), size(0), capacity(0) {}

Array::~Array() {
    delete[] data;
}


// перевыделение
void Array::resize(size_t new_cap) {
    string* new_data = new string[new_cap];
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_cap;
}


// вставка в конец
void Array::insertEnd(string val) {
    if (size == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[size++] = val;
}


// вставка по индексу
void Array::insertAt(int idx, string val) {
    if (idx < 0 || idx > (int)size) {
        cout << "Index out of range" << endl;
        return;
    }
    if (size == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    for (size_t i = size; i > (size_t)idx; --i) {
        data[i] = data[i - 1];
    }
    data[idx] = val;
    size++;
}


// удаление по индексу
void Array::removeAt(int idx) {
    if (idx < 0 || idx >= (int)size) {
        cout << "Index out of range" << endl;
        return;
    }
    for (size_t i = idx; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    size--;
}

// получ эла по индексу
string Array::get(int idx) {
    if (idx < 0 || idx >= (int)size) {
        return "ERROR";
    }
    return data[idx];
}

// замена эла по индексу
void Array::set(int idx, string val) {
    if (idx < 0 || idx >= (int)size) {
        cout << "Index out of range" << endl;
        return;
    }
    data[idx] = val;
}

// тут понятно
int Array::length() {
    return size;
}

void Array::print() {
    cout << "Array[" << size << "]: ";
    for (size_t i = 0; i < size; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;
}

void Array::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "MPUSH") {
        if (tokens.size() == 3) {
            insertEnd(tokens[2]);
        } else if (tokens.size() == 4) {
            int idx = stoi(tokens[2]);
            insertAt(idx, tokens[3]);
        }
    } else if (cmd == "MDEL") {
        if (tokens.size() < 3) { cout << "MDEL: missing index" << endl; return; }
        removeAt(stoi(tokens[2]));
    } else if (cmd == "MGET") {
        if (tokens.size() < 3) { cout << "MGET: missing index" << endl; return; }
        cout << get(stoi(tokens[2])) << endl;
    } else if (cmd == "MSET") {
        if (tokens.size() < 4) { cout << "MSET: missing args" << endl; return; }
        set(stoi(tokens[2]), tokens[3]);
    } else if (cmd == "MLEN") {
        cout << length() << endl;
    }
}
char Array::getType() const { return 'M'; }

void Array::save(ostream& out) const {
    out << size;
    for (size_t i = 0; i < size; ++i) {
        out << " " << data[i];
    }
}

void Array::load(istream& in) {
    delete[] data; data = nullptr; size = 0; capacity = 0;
    int count;
    if (!(in >> count)) return;
    for (int i = 0; i < count; ++i) {
        string v;
        if (!(in >> v)) break;
        insertEnd(v);
    }
}