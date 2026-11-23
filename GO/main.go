package main

import (
    "bufio"
    "fmt"
    "os"
    "strings"

    "lab2_go/lab2_tasks"
    "lab2_go/structures"
)

func printHelp(target string) {
    switch target {

    case "LOGIC", "LOGIC_EVAL":
        fmt.Println("=== ЛОГИЧЕСКИЙ КАЛЬКУЛЯТОР (LogicEval) ===")
        fmt.Println("LOGIC_EVAL <expression>")
        fmt.Println("Example: LOGIC_EVAL 1 & 0 | 1")

    case "SET", "SETADD", "SETDEL", "SET_AT":
        fmt.Println("=== МНОЖЕСТВО (Set) ===")
        fmt.Println("SETADD <value>")
        fmt.Println("SETDEL <value>")
        fmt.Println("SET_AT <value>")

    case "SETS", "SETS_NEW", "SETS_MAX":
        fmt.Println("=== МАССИВ МНОЖЕСТВ (MaxIntersect) ===")
        fmt.Println("SETS_NEW")
        fmt.Println("SETS_ADD <index> <value>")
        fmt.Println("SETS_MAX")
        fmt.Println("SETS_PRINT")

    case "PIRAM", "PIRAM_ADD", "PIRAM_CALC":
        fmt.Println("=== ПИРАМИДА ИЗ БЛОКОВ (Pyramid) ===")
        fmt.Println("PIRAM_ADD <width> <height>")
        fmt.Println("PIRAM_CALC")
        fmt.Println("PIRAM_CLEAR")

    case "TREE", "TREE_FORKS":
        fmt.Println("=== ДЕРЕВО ПОИСКА (TreeForks) ===")
        fmt.Println("TREE_FORKS <numbers...> 0")

    case "HT", "CK", "HT_PUT", "CK_PUT":
        fmt.Println("=== ХЕШ-ТАБЛИЦА (HashTable) ===")
        fmt.Println("HT_PUT <key> <value>")
        fmt.Println("HT_GET <key>")
        fmt.Println("HT_DEL <key>")
        fmt.Println("CK_PUT <key> <value>")
        fmt.Println("CK_GET <key>")
        fmt.Println("HT_ANALYZE")

    case "ROUTE", "ROUTE_ADD", "ROUTE_FIND":
        fmt.Println("=== ОПТИМИЗАТОР МАРШРУТОВ ===")
        fmt.Println("ROUTE_ADD <from> <to> <delay>")
        fmt.Println("ROUTE_FIND <from> <to>")

    case "LFU", "LFU_SET", "LFU_GET":
        fmt.Println("=== LFU КЭШ ===")
        fmt.Println("LFU_SET <key> <value>")
        fmt.Println("LFU_GET <key>")
        fmt.Println("LFU_CAP <capacity>")

    default:
        fmt.Println("=== ДОСТУПНЫЕ КОМАНДЫ (LAB 2) ===")
        fmt.Println("HELP LOGIC")
        fmt.Println("HELP SET")
        fmt.Println("HELP SETS")
        fmt.Println("HELP PIRAM")
        fmt.Println("HELP TREE")
        fmt.Println("HELP HT")
        fmt.Println("HELP ROUTE")
        fmt.Println("HELP LFU")
        fmt.Println("EXIT")
    }
}

func dispatch(
    cmd string,
    le *lab2_tasks.LogicEval,
    st *lab2_tasks.Set,
    mi *lab2_tasks.MaxIntersect,
    py *lab2_tasks.Pyramid,
    tf *lab2_tasks.TreeForks,
    ht *lab2_tasks.HashTable,
    ro *lab2_tasks.RouteOptimizer,
    lf *lab2_tasks.LFUCache,
    tokens structures.TokenList,
) bool {

    switch {
    case strings.HasPrefix(cmd, "LOGIC_"):
        le.Execute(cmd, tokens)

    case cmd == "SETADD" ||
        cmd == "SETDEL" ||
        cmd == "SET_AT":

        st.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "SETS_"):
        mi.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "PIRAM_"):
        py.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "TREE_"):
        tf.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "HT_") ||
        strings.HasPrefix(cmd, "CK_"):

        ht.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "ROUTE_"):
        ro.Execute(cmd, tokens)

    case strings.HasPrefix(cmd, "LFU_"):
        lf.Execute(cmd, tokens)

    default:
        fmt.Println("Unknown command:", cmd)
        return false
    }

    return true
}

func main() {
    file := ""
    query := ""
    lfuCapacity := 3

    args := os.Args

    for i := 1; i < len(args); i++ {
        switch args[i] {

        case "--file":
            if i+1 < len(args) {
                i++
                file = args[i]
            }

        case "--query":
            if i+1 < len(args) {
                i++
                query = args[i]
            }

        case "--capacity":
            if i+1 < len(args) {
                i++
                fmt.Sscan(args[i], &lfuCapacity)
            }
        }
    }

    le := lab2_tasks.NewLogicEval()
    st := lab2_tasks.NewSet(16)
    mi := lab2_tasks.NewMaxIntersect()
    py := lab2_tasks.NewPyramid()
    tf := lab2_tasks.NewTreeForks()
    ht := lab2_tasks.NewHashTable(16)
    ro := lab2_tasks.NewRouteOptimizer()
    lf := lab2_tasks.NewLFUCache(lfuCapacity)

    var input *os.File

    if file != "" {
        f, err := os.Open(file)

        if err != nil {
            fmt.Fprintln(os.Stderr, "Cannot open:", file)
            os.Exit(1)
        }

        defer f.Close()

        input = f
    } else {
        input = os.Stdin
    }

    scanner := bufio.NewScanner(input)

    for scanner.Scan() {
        line := scanner.Text()

        if line == "" || strings.HasPrefix(line, "#") {
            continue
        }

        tokens := strings.Fields(line)

        if len(tokens) == 0 {
            continue
        }

        cmd := tokens[0]

        if cmd == "HELP" {
            target := ""

            if len(tokens) > 1 {
                target = strings.ToUpper(tokens[1])
            }

            printHelp(target)
            continue
        }

        if cmd == "EXIT" {
            break
        }

        dispatch(
            cmd,
            le,
            st,
            mi,
            py,
            tf,
            ht,
            ro,
            lf,
            structures.TokenList(tokens),
        )
    }

    if err := scanner.Err(); err != nil {
        fmt.Fprintln(os.Stderr, err)
    }

    if query != "" {
        tokens := strings.Fields(query)

        if len(tokens) > 0 {
            dispatch(
                tokens[0],
                le,
                st,
                mi,
                py,
                tf,
                ht,
                ro,
                lf,
                structures.TokenList(tokens),
            )
        }
    }
}