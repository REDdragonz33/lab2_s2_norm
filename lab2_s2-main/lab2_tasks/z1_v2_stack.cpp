#include "z1_v2_stack.h"
#include <iostream>

// конструктор
LogicEval::LogicEval() {}

// деструктор
LogicEval::~LogicEval() {}

// приоритет операций
int LogicEval::prec(char op) {
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|' || op == '^') return 1;
    return 0;
}

// применяем оператор
void LogicEval::applyOp() {
    string opS = ops.remove();
    if (opS == "EMPTY") return;

    char op = opS[0];
    if (op == '!') {
        string a = vals.remove();
        if (a == "EMPTY") {
            vals.insert("ERROR");
            return;
        }
        vals.insert(a == "0" ? "1" : "0");
    } else {
        string b = vals.remove();
        string a = vals.remove();
        if (a == "EMPTY" || b == "EMPTY") {
            vals.insert("ERROR");
            return;
        }
        int x = stoi(a);
        int y = stoi(b);
        int r = 0;

        if (op == '&') r = x & y;
        else if (op == '|') r = x | y;
        else if (op == '^') r = x ^ y;

        vals.insert(to_string(r));
    }
}

// выводим
void LogicEval::print() {
    cout << "Logic evaluator ready" << endl;
}

// команди
void LogicEval::execute(const string& cmd, const TokenList& tokens) {
    if (cmd != "LOGIC_EVAL") return;
    if (tokens.size() < 2) {
        cout << "LOGIC_EVAL: missing expression" << endl;
        return;
    }

    // очищаем стеки
    while (vals.remove() != "EMPTY") {}
    while (ops.remove() != "EMPTY") {}

    // склеиваем выражение на случай пробелов
    string e = "";
    for (int i = 1; i < tokens.size(); ++i) {
        if (i > 1) e += ' ';
        e += tokens[i];
    }

    bool expectOperand = true;
    bool valid = true;

    for (size_t i = 0; i < e.size() && valid; ++i) {
        char c = e[i];
        if (c == ' ' || c == '\t') continue;

        if (c == '0' || c == '1') {
            if (!expectOperand) { valid = false; break; }
            vals.insert(string(1, c));
            expectOperand = false;

            while (true) {
                string top = ops.remove();
                if (top == "EMPTY") break;
                if (top == "!") {
                    ops.insert(top);
                    applyOp();
                } else {
                    ops.insert(top);
                    break;
                }
            }
        } 
        else if (c == '(') {
            if (!expectOperand) { valid = false; break; }
            ops.insert("(");
            expectOperand = true;
        } 
        else if (c == ')') {
            if (expectOperand) { valid = false; break; }
            bool foundOpen = false;
            
            while (true) {
                string top = ops.remove();
                if (top == "EMPTY") break;
                if (top == "(") {
                    foundOpen = true;
                    break;
                }
                ops.insert(top);
                applyOp();
            }
            if (!foundOpen) { valid = false; break; }

            while (true) {
                string top = ops.remove();
                if (top == "EMPTY") break;
                if (top == "!") {
                    ops.insert(top);
                    applyOp();
                } else {
                    ops.insert(top);
                    break;
                }
            }
            expectOperand = false;
        } 
        else if (c == '!' || c == '&' || c == '|' || c == '^') {
            if (c != '!' && expectOperand) { valid = false; break; }
            if (c == '!' && !expectOperand) { valid = false; break; }

            while (true) {
                string top = ops.remove();
                if (top == "EMPTY" || top == "(") {
                    if (top == "(") ops.insert(top);
                    break;
                }
                char t = top[0];
                if (prec(t) > prec(c) || (prec(t) == prec(c) && c != '!')) {
                    ops.insert(top);
                    applyOp();
                } else {
                    ops.insert(top);
                    break;
                }
            }
            ops.insert(string(1, c));
            expectOperand = true;
        } 
        else {
            valid = false;
        }
    }

    if (expectOperand) valid = false;

    while (valid) {
        string top = ops.remove();
        if (top == "EMPTY") break;
        if (top == "(") {
            valid = false;
            break;
        }
        ops.insert(top);
        applyOp();
    }

    string result = vals.remove();
    if (vals.remove() != "EMPTY" || result == "EMPTY" || result == "ERROR") {
        valid = false;
    }

    if (valid) {
        cout << "Result: " << result << endl;
    } else {
        cout << "Incorrect expression" << endl;
    }
}

// сохранялко
void LogicEval::save(ostream& out) const {
    out << "LOGIC";
}

// загрузко
void LogicEval::load(istream& in) {
    string d;
    in >> d;
}

// тип
char LogicEval::getType() const {
    return '1';
}