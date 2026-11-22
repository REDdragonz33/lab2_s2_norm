#include "z6_v4_vv2_opti.h"
#include "../structures/dlist.h"
#include <iostream>
#include <climits>

// конструктор
RouteOptimizer::RouteOptimizer() {}

// деструктор
RouteOptimizer::~RouteOptimizer() {}

// добавить маршрут
void RouteOptimizer::addRoute(const string& a, const string& b, long long delay) {
    if (delay < 0) {
        cout << "Negative delay is not supported" << endl;
        return;
    }
    ht.chainPut(a + "->" + b, to_string(delay));
}

// алгоритм Дейкстры
long long RouteOptimizer::dijkstra(const string& src, const string& dst) {
    DList edges;
    ht.collectEdges("", edges);

    int edgeCount = 0;
    for (DNode* e = edges.getHead(); e; e = e->next) {
        ++edgeCount;
    }

    int cap = edgeCount * 2 + 2;
    int n = 0;

    string* nodes = new string[cap];
    long long* dist = new long long[cap];
    bool* used = new bool[cap];
    int* prev = new int[cap];

    for (int i = 0; i < cap; ++i) {
        dist[i] = LLONG_MAX;
        used[i] = false;
        prev[i] = -1;
    }

    // лямбда для добавления узла
    auto addNode = [&](const string& name) -> int {
        for (int i = 0; i < n; ++i) {
            if (nodes[i] == name) return i;
        }
        nodes[n] = name;
        return n++;
    };

    int source = addNode(src);
    int target = addNode(dst);

    // собираем все упомянутые узлы из ребер
    for (DNode* e = edges.getHead(); e; e = e->next) {
        string s = e->data;
        size_t p = s.find("->");
        size_t q = s.find(':', p == string::npos ? 0 : p + 2);
        
        if (p != string::npos && q != string::npos) {
            addNode(s.substr(0, p));
            addNode(s.substr(p + 2, q - p - 2));
        }
    }

    dist[source] = 0;

    // основной цикл Дейкстры
    for (int step = 0; step < n; ++step) {
        int u = -1;
        long long best = LLONG_MAX;

        // ищем ближайшую непосещенную вершину
        for (int i = 0; i < n; ++i) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        if (u < 0) break;
        used[u] = true;
        if (u == target) break;

        // обновляем расстояния до соседей
        string prefix = nodes[u] + "->";
        DList outgoing;
        ht.collectEdges(prefix, outgoing);

        for (DNode* e = outgoing.getHead(); e; e = e->next) {
            string s = e->data;
            size_t p = s.find("->");
            size_t q = s.find(':', p + 2);
            
            if (p == string::npos || q == string::npos) continue;

            string v = s.substr(p + 2, q - p - 2);
            long long weight = stoll(s.substr(q + 1));

            int vi = -1;
            for (int j = 0; j < n; ++j) {
                if (nodes[j] == v) {
                    vi = j;
                    break;
                }
            }

            if (vi >= 0 && best <= LLONG_MAX - weight && best + weight < dist[vi]) {
                dist[vi] = best + weight;
                prev[vi] = u;
            }
        }
    }

    long long answer = dist[target];

    // выводим результат
    if (answer == LLONG_MAX) {
        cout << "No route" << endl;
    } else {
        int* path = new int[n];
        int len = 0;
        
        for (int v = target; v != -1; v = prev[v]) {
            path[len++] = v;
        }

        cout << "Route: ";
        for (int i = len - 1; i >= 0; --i) {
            cout << nodes[path[i]];
            if (i > 0) cout << " -> ";
        }
        cout << "; minimum delay = " << answer << endl;
        
        delete[] path;
    }

    delete[] nodes;
    delete[] dist;
    delete[] used;
    delete[] prev;

    return (answer == LLONG_MAX) ? -1 : answer;
}

// выводим
void RouteOptimizer::print() {
    cout << "RouteOptimizer ready" << endl;
}

// команди
void RouteOptimizer::execute(const string& cmd, const TokenList& tokens) {
    if (cmd == "ROUTE_ADD") {
        if (tokens.size() < 4) {
            cout << "ROUTE_ADD: missing args" << endl;
            return;
        }
        addRoute(tokens[1], tokens[2], stoll(tokens[3]));
    } 
    else if (cmd == "ROUTE_FIND") {
        if (tokens.size() < 3) {
            cout << "ROUTE_FIND: missing args" << endl;
            return;
        }
        dijkstra(tokens[1], tokens[2]);
    }
}

// сохранялко
void RouteOptimizer::save(ostream& out) const {
    out << "ROUTE";
}

// загрузко
void RouteOptimizer::load(istream& in) {
    string d;
    in >> d;
}

// тип
char RouteOptimizer::getType() const {
    return 'O';
}