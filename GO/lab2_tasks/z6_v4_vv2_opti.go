package lab2_tasks

import (
    "fmt"
    "math"

    "lab2_go/structures"
)

type RouteOptimizer struct {
    ht *HashTable
}

func NewRouteOptimizer() *RouteOptimizer {
    return &RouteOptimizer{
        ht: NewHashTable(16),
    }
}

// Добавление маршрута.
func (r *RouteOptimizer) AddRoute(
    a string,
    b string,
    delay int64,
) {
    if delay < 0 {
        fmt.Println("Negative delay is not supported")
        return
    }

    key := a + "->" + b

    r.ht.ChainPut(key, fmt.Sprint(delay))
}

// Поиск индекса сервера.
func findNode(nodes []string, name string) int {
    for i := 0; i < len(nodes); i++ {
        if nodes[i] == name {
            return i
        }
    }

    return -1
}

// Добавление сервера.
func addNode(nodes *[]string, name string) int {
    index := findNode(*nodes, name)

    if index >= 0 {
        return index
    }

    *nodes = append(*nodes, name)

    return len(*nodes) - 1
}

// Алгоритм Дейкстры.
func (r *RouteOptimizer) Dijkstra(
    src string,
    dst string,
) int64 {

    edges := structures.NewDList()

    r.ht.CollectEdges("", edges)

    edgeCount := edges.GetSize()

    capacity := edgeCount*2 + 2

    nodes := make([]string, 0, capacity)

    source := addNode(&nodes, src)
    target := addNode(&nodes, dst)

    // Собираем все вершины графа.
    for e := edges.GetHead(); e != nil; e = e.Next {
        value := e.Data

        arrow := -1

        for i := 0; i+1 < len(value); i++ {
            if value[i:i+2] == "->" {
                arrow = i
                break
            }
        }

        if arrow < 0 {
            continue
        }

        colon := -1

        for i := arrow + 2; i < len(value); i++ {
            if value[i] == ':' {
                colon = i
                break
            }
        }

        if colon < 0 {
            continue
        }

        from := value[:arrow]
        to := value[arrow+2 : colon]

        addNode(&nodes, from)
        addNode(&nodes, to)
    }

    n := len(nodes)

    dist := make([]int64, n)
    used := make([]bool, n)
    prev := make([]int, n)

    for i := 0; i < n; i++ {
        dist[i] = math.MaxInt64
        prev[i] = -1
    }

    dist[source] = 0

    // Основной цикл алгоритма.
    for step := 0; step < n; step++ {
        u := -1
        best := int64(math.MaxInt64)

        for i := 0; i < n; i++ {
            if !used[i] && dist[i] < best {
                best = dist[i]
                u = i
            }
        }

        if u < 0 {
            break
        }

        used[u] = true

        if u == target {
            break
        }

        prefix := nodes[u] + "->"

        outgoing := structures.NewDList()

        r.ht.CollectEdges(prefix, outgoing)

        for e := outgoing.GetHead(); e != nil; e = e.Next {
            value := e.Data

            arrow := -1

            for i := 0; i+1 < len(value); i++ {
                if value[i:i+2] == "->" {
                    arrow = i
                    break
                }
            }

            if arrow < 0 {
                continue
            }

            colon := -1

            for i := arrow + 2; i < len(value); i++ {
                if value[i] == ':' {
                    colon = i
                    break
                }
            }

            if colon < 0 {
                continue
            }

            to := value[arrow+2 : colon]

            var weight int64
            fmt.Sscan(value[colon+1:], &weight)

            vi := findNode(nodes, to)

            if vi < 0 || weight < 0 {
                continue
            }

            if best > math.MaxInt64-weight {
                continue
            }

            newDistance := best + weight

            if newDistance < dist[vi] {
                dist[vi] = newDistance
                prev[vi] = u
            }
        }
    }

    answer := dist[target]

    if answer == math.MaxInt64 {
        fmt.Println("No route")
        return -1
    }

    // Восстанавливаем маршрут.
    path := []int{}

    for v := target; v != -1; v = prev[v] {
        path = append(path, v)

        if v == source {
            break
        }
    }

    fmt.Print("Route: ")

    for i := len(path) - 1; i >= 0; i-- {
        fmt.Print(nodes[path[i]])

        if i > 0 {
            fmt.Print(" -> ")
        }
    }

    fmt.Printf(
        "; minimum delay = %d\n",
        answer,
    )

    return answer
}

func (r *RouteOptimizer) Print() {
    fmt.Println("RouteOptimizer ready")
}

func (r *RouteOptimizer) Execute(
    cmd string,
    tokens structures.TokenList,
) {
    switch cmd {
    case "ROUTE_ADD":
        if len(tokens) < 4 {
            fmt.Println("ROUTE_ADD: missing args")
            return
        }

        var delay int64

        fmt.Sscan(tokens[3], &delay)

        r.AddRoute(
            tokens[1],
            tokens[2],
            delay,
        )

    case "ROUTE_FIND":
        if len(tokens) < 3 {
            fmt.Println("ROUTE_FIND: missing args")
            return
        }

        r.Dijkstra(tokens[1], tokens[2])
    }
}

func (r *RouteOptimizer) Save() string {
    return "ROUTE"
}

func (r *RouteOptimizer) Load(data string) {
}

func (r *RouteOptimizer) GetType() byte {
    return 'O'
}