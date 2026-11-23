package lab2_tasks

import (
    "fmt"

    "lab2_go/structures"
)

type MaxIntersect struct {
    sets    []*Set
    count   int
    maxSets int
}

func NewMaxIntersect() *MaxIntersect {
    return &MaxIntersect{
        sets:    make([]*Set, 16),
        count:   0,
        maxSets: 16,
    }
}

func (m *MaxIntersect) NewSet() {
    if m.count == m.maxSets {
        newCapacity := m.maxSets * 2

        bigger := make([]*Set, newCapacity)

        copy(bigger, m.sets)

        m.sets = bigger
        m.maxSets = newCapacity
    }

    m.sets[m.count] = NewSet(16)
    m.count++

    fmt.Println("Set", m.count-1, "created")
}

func (m *MaxIntersect) AddElem(index int, value string) {
    if index < 0 || index >= m.count {
        fmt.Println("bad set idx")
        return
    }

    m.sets[index].Add(value)
}

func (m *MaxIntersect) printSet(set *Set) {
    fmt.Print("{")

    for i := 0; i < set.Size(); i++ {
        if i > 0 {
            fmt.Print(", ")
        }

        fmt.Print(set.GetAt(i))
    }

    fmt.Print("}")
}

func (m *MaxIntersect) FindMax() {
    if m.count < 2 {
        fmt.Println("Need >= 2 sets")
        return
    }

    best := -1
    bi := -1
    bj := -1

    for i := 0; i < m.count; i++ {
        for j := i + 1; j < m.count; j++ {

            small := m.sets[i]
            big := m.sets[j]

            if small.Size() > big.Size() {
                small, big = big, small
            }

            common := NewSet(8)

            for k := 0; k < small.Size(); k++ {
                value := small.GetAt(k)

                if big.Has(value) {
                    common.Add(value)
                }
            }

            if common.Size() > best {
                best = common.Size()
                bi = i
                bj = j
            }
        }
    }

    fmt.Println("Max common =", best)

    fmt.Print("Sets: ")
    m.printSet(m.sets[bi])

    fmt.Print(" and ")
    m.printSet(m.sets[bj])

    fmt.Println()

    commonSet := NewSet(8)

    small := m.sets[bi]
    big := m.sets[bj]

    if small.Size() > big.Size() {
        small, big = big, small
    }

    for i := 0; i < small.Size(); i++ {
        value := small.GetAt(i)

        if big.Has(value) {
            commonSet.Add(value)
        }
    }

    fmt.Print("Common elements: ")
    m.printSet(commonSet)
    fmt.Println()
}

func (m *MaxIntersect) Print() {
    fmt.Println("MaxIntersect ready, sets=", m.count)

    for i := 0; i < m.count; i++ {
        fmt.Printf("%d: ", i)

        m.printSet(m.sets[i])

        fmt.Println()
    }
}

func (m *MaxIntersect) Execute(cmd string, tokens structures.TokenList) {
    switch cmd {
    case "SETS_NEW":
        m.NewSet()

    case "SETS_ADD":
        if len(tokens) < 3 {
            fmt.Println("SETS_ADD: missing args")
            return
        }

        var index int
        fmt.Sscan(tokens[1], &index)

        m.AddElem(index, tokens[2])

    case "SETS_MAX":
        m.FindMax()

    case "SETS_PRINT":
        m.Print()
    }
}

func (m *MaxIntersect) Save() string {
    result := fmt.Sprintln(m.count)

    for i := 0; i < m.count; i++ {
        result += m.sets[i].Save() + "\n"
    }

    return result
}

func (m *MaxIntersect) Load(data string) {
    m.sets = make([]*Set, 16)
    m.count = 0
    m.maxSets = 16

    fields := splitLines(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 0; i < count && i+1 < len(fields); i++ {
        m.NewSet()
        m.sets[m.count-1].Load(fields[i+1])
    }
}

func (m *MaxIntersect) GetType() byte {
    return '3'
}