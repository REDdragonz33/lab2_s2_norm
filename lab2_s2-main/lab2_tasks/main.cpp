#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>

#include "../structures/istructure.h"
#include "z1_v2_stack.h"
#include "z2_atd.h"
#include "z3_v4_array.h"
#include "z4_v4_piram.h"
#include "z5_v4_tree.h"
#include "z6_v2_vv1_doublehash.h"
#include "z6_v4_vv2_opti.h"
#include "z7_v2_lfu.h"

using namespace std;

// Функция справки (в стиле 1-й лабы)
void printHelp(const string& type) {
    if (type == "LOGIC" || type == "LOGIC_EVAL") {
        cout << "=== ЛОГИЧЕСКИЙ КАЛЬКУЛЯТОР (LogicEval) ===" << endl;
        cout << "LOGIC_EVAL <выражение>  - Вычислить логическое выражение (0, 1, &, |, ^, !)" << endl;
        cout << "Пример: LOGIC_EVAL 1 & 0 | 1" << endl;
    } 
    else if (type == "SET" || type == "SETADD" || type == "SETDEL" || type == "SET_AT") {
        cout << "=== МНОЖЕСТВО (Set) ===" << endl;
        cout << "SETADD <значение>       - Добавить элемент в множество" << endl;
        cout << "SETDEL <значение>       - Удалить элемент из множества" << endl;
        cout << "SET_AT <значение>       - Проверить наличие элемента (TRUE/FALSE)" << endl;
    }
    else if (type == "SETS" || type == "SETS_NEW" || type == "SETS_MAX") {
        cout << "=== МАССИВ МНОЖЕСТВ (MaxIntersect) ===" << endl;
        cout << "SETS_NEW                - Создать новое пустое множество" << endl;
        cout << "SETS_ADD <индекс> <значение> - Добавить элемент в множество по индексу" << endl;
        cout << "SETS_MAX                - Найти пару множеств с максимальным пересечением" << endl;
        cout << "SETS_PRINT              - Вывести все созданные множества" << endl;
    }
    else if (type == "PIRAM" || type == "PIRAM_ADD" || type == "PIRAM_CALC") {
        cout << "=== ПИРАМИДА ИЗ БЛОКОВ (Pyramid) ===" << endl;
        cout << "PIRAM_ADD <ширина> <высота> - Добавить блок заданного размера" << endl;
        cout << "PIRAM_CALC              - Рассчитать и вывести максимальную высоту пирамиды" << endl;
        cout << "PIRAM_CLEAR             - Очистить все добавленные блоки" << endl;
    }
    else if (type == "TREE" || type == "TREE_FORKS") {
        cout << "=== ДЕРЕВО ПОИСКА (TreeForks) ===" << endl;
        cout << "TREE_FORKS <число1> <число2> ... 0 - Построить дерево и найти узлы с двумя детьми" << endl;
        cout << "(Ввод чисел заканчивается нулем)" << endl;
    }
    else if (type == "HT" || type == "CK" || type == "HT_PUT" || type == "CK_PUT") {
        cout << "=== ХЭШ-ТАБЛИЦА (HashTable) ===" << endl;
        cout << "HT_PUT <ключ> <значение>    - Вставка (метод цепочек)" << endl;
        cout << "HT_GET <ключ>               - Поиск (метод цепочек)" << endl;
        cout << "HT_DEL <ключ>               - Удаление (метод цепочек)" << endl;
        cout << "CK_PUT <ключ> <значение>    - Вставка (кукушкино хеширование)" << endl;
        cout << "CK_GET <ключ>               - Поиск (кукушкино хеширование)" << endl;
        cout << "HT_ANALYZE              - Эмпирический анализ и сравнение методов" << endl;
    }
    else if (type == "ROUTE" || type == "ROUTE_ADD" || type == "ROUTE_FIND") {
        cout << "=== ОПТИМИЗАТОР МАРШРУТОВ (RouteOptimizer) ===" << endl;
        cout << "ROUTE_ADD <откуда> <куда> <задержка> - Добавить направленное ребро графа" << endl;
        cout << "ROUTE_FIND <откуда> <куда>           - Найти кратчайший путь (алгоритм Дейкстры)" << endl;
    }
    else if (type == "LFU" || type == "LFU_SET" || type == "LFU_GET") {
        cout << "=== LFU КЭШ (LFUCache) ===" << endl;
        cout << "LFU_SET <ключ> <значение> - Добавить или обновить элемент в кэше" << endl;
        cout << "LFU_GET <ключ>            - Получить элемент (увеличивает частоту использования)" << endl;
        cout << "LFU_CAP <емкость>         - Изменить максимальную емкость кэша" << endl;
    }
    else {
        cout << "=== ДОСТУПНЫЕ КОМАНДЫ И СТРУКТУРЫ (LAB 2) ===" << endl;
        cout << "HELP LOGIC   - Справка по логическому калькулятору" << endl;
        cout << "HELP SET     - Справка по множеству (открытая адресация)" << endl;
        cout << "HELP SETS    - Справка по массиву множеств (пересечения)" << endl;
        cout << "HELP PIRAM   - Справка по задаче о пирамиде из блоков" << endl;
        cout << "HELP TREE    - Справка по дереву поиска (узлы с двумя детьми)" << endl;
        cout << "HELP HT      - Справка по хэш-таблицам (цепочки и кукушка)" << endl;
        cout << "HELP ROUTE   - Справка по оптимизатору маршрутов (Дейкстра)" << endl;
        cout << "HELP LFU     - Справка по LFU кэшу" << endl;
        cout << "EXIT         - Завершить работу программы" << endl;
    }
}

IStructure* dispatch(const string& cmd,
    LogicEval& le, Set& st, MaxIntersect& mi, Pyramid& py,
    TreeForks& tf, HashTable& ht, RouteOptimizer& ro, LFUCache& lf) {
    if (cmd.rfind("LOGIC_", 0) == 0) return &le;
    if (cmd == "SETADD" || cmd == "SETDEL" || cmd == "SET_AT") return &st;
    if (cmd.rfind("SETS_", 0) == 0) return &mi;
    if (cmd.rfind("PIRAM_", 0) == 0) return &py;
    if (cmd.rfind("TREE_", 0) == 0) return &tf;
    if (cmd.rfind("HT_", 0) == 0 || cmd.rfind("CK_", 0) == 0) return &ht;
    if (cmd.rfind("ROUTE_", 0) == 0) return &ro;
    if (cmd.rfind("LFU_", 0) == 0) return &lf;
    return nullptr;
}

int main(int argc, char** argv) {
    string file = "", query = "";
    int lfuCapacity = 3;
    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "--file" && i + 1 < argc) file = argv[++i];
        else if (a == "--query" && i + 1 < argc) query = argv[++i];
        else if (a == "--capacity" && i + 1 < argc) lfuCapacity = stoi(argv[++i]);
    }

    LogicEval le;
    Set st;
    MaxIntersect mi;
    Pyramid py;
    TreeForks tf;
    HashTable ht;
    RouteOptimizer ro;
    LFUCache lf(lfuCapacity);

    istream* in = &cin;
    ifstream fin;
    if (!file.empty()) {
        fin.open(file);
        if (!fin.is_open()) {
            cerr << "Cannot open: " << file << endl;
            return 1;
        }
        in = &fin;
    }

    string line;
    while (getline(*in, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        stringstream ss(line);
        TokenList tokens; 
        string t;
        while (ss >> t) tokens.push_back(t);
        if (tokens.empty()) continue;

        // Обработка команды HELP
        if (tokens[0] == "HELP") {
            string target = (tokens.size() > 1) ? tokens[1] : "";
            for (char& c : target) c = toupper(c);
            printHelp(target);
            continue;
        }

        // Обработка команды EXIT
        if (tokens[0] == "EXIT") {
            break;
        }

        IStructure* obj = dispatch(tokens[0], le, st, mi, py, tf, ht, ro, lf);
        if (obj) {
            try {
                obj->execute(tokens[0], tokens);
            } catch (const exception& ex) {
                cout << "Invalid command arguments: " << ex.what() << endl;
            }
        } else {
            cout << "Unknown command: " << tokens[0] << endl;
        }
    }

    if (!query.empty()) {
        stringstream ss(query);
        TokenList tokens; 
        string t;
        while (ss >> t) tokens.push_back(t);
        if (!tokens.empty()) {
            IStructure* obj = dispatch(tokens[0], le, st, mi, py, tf, ht, ro, lf);
            if (obj) {
                try {
                    obj->execute(tokens[0], tokens);
                } catch (const exception& ex) {
                    cout << "Invalid command arguments: " << ex.what() << endl;
                }
            }
        }
    }
    return 0;
}