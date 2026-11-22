#include "z2_atd.h"
#include <iostream>

// конструктор
Set::Set(int cap) : capacity(cap < 8 ? 8 : cap), cnt(0) {
    table = new SetEntry[capacity];
}

// деструктор
Set::~Set() {
    delete[] table;
}

// хеш-функция
int Set::hashFunc(const string& val) const {
    unsigned long long h = 0;
    for (unsigned char c : val) {
        h = (h * 31 + c) % (unsigned long long)capacity;
    }
    return (int)h;
}

// проверка наличия
bool Set::has(const string& val) const {
    int start = hashFunc(val);
    for (int step = 0; step < capacity; ++step) {
        int i = (start + step) % capacity;
        if (!table[i].occupied && !table[i].deleted) {
            return false;
        }
        if (table[i].occupied && table[i].value == val) {
            return true;
        }
    }
    return false;
}

// рехеш
void Set::rehash() {
    SetEntry* old = table;
    int oldCap = capacity;
    capacity *= 2;
    table = new SetEntry[capacity];
    cnt = 0;
    for (int i = 0; i < oldCap; ++i) {
        if (old[i].occupied) {
            add(old[i].value);
        }
    }
    delete[] old;
}

// вставка
void Set::add(const string& val) {
    if ((cnt + 1) * 10 >= capacity * 7) {
        rehash();
    }
    int start = hashFunc(val);
    int firstDeleted = -1;
    for (int step = 0; step < capacity; ++step) {
        int i = (start + step) % capacity;
        if (table[i].occupied && table[i].value == val) {
            return;
        }
        if (!table[i].occupied && table[i].deleted && firstDeleted < 0) {
            firstDeleted = i;
        }
        if (!table[i].occupied && !table[i].deleted) {
            if (firstDeleted >= 0) {
                i = firstDeleted;
            }
            table[i].value = val;
            table[i].occupied = true;
            table[i].deleted = false;
            ++cnt;
            return;
        }
    }
    if (firstDeleted >= 0) {
        table[firstDeleted].value = val;
        table[firstDeleted].occupied = true;
        table[firstDeleted].deleted = false;
        ++cnt;
    }
}

// удаление
void Set::del(const string& val) {
    int start = hashFunc(val);
    for (int step = 0; step < capacity; ++step) {
        int i = (start + step) % capacity;
        if (!table[i].occupied && !table[i].deleted) {
            return;
        }
        if (table[i].occupied && table[i].value == val) {
            table[i].occupied = false;
            table[i].deleted = true;
            table[i].value.clear();
            --cnt;
            return;
        }
    }
}

// размер
int Set::size() const {
    return cnt;
}

// получение по индексу
string Set::getAt(int index) const {
    if (index < 0) return "";
    for (int i = 0, seen = 0; i < capacity; ++i) {
        if (table[i].occupied && seen++ == index) {
            return table[i].value;
        }
    }
    return "";
}

// выводим
void Set::print() {
    cout << "Set size=" << cnt << ": ";
    for (int i = 0; i < cnt; ++i) {
        cout << getAt(i) << ' ';
    }
    cout << endl;
}

// команди
void Set::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "SETADD") {
        if (tokens.size() < 2) {
            cout << "SETADD: missing value" << endl;
            return;
        }
        add(tokens[1]);
    } else if (cmd == "SETDEL") {
        if (tokens.size() < 2) {
            cout << "SETDEL: missing value" << endl;
            return;
        }
        del(tokens[1]);
    } else if (cmd == "SET_AT") {
        if (tokens.size() < 2) {
            cout << "SET_AT: missing value" << endl;
            return;
        }
        if (has(tokens[1])) {
            cout << "TRUE" << endl;
        } else {
            cout << "FALSE" << endl;
        }
    }
}

// сохранялко
void Set::save(ostream& out) const {
    out << cnt;
    for (int i = 0; i < cnt; ++i) {
        out << ' ' << getAt(i);
    }
}

// загрузко
void Set::load(istream& in) {
    delete[] table;
    table = new SetEntry[capacity];
    cnt = 0;
    int n;
    if (!(in >> n)) return;
    for (int i = 0; i < n; ++i) {
        string v;
        if (!(in >> v)) break;
        add(v);
    }
}

// тип
char Set::getType() const {
    return '2';
}