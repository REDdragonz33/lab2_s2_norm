package lab2_tasks

import (
    "fmt"

    "lab2_go/structures"
)

type SetEntry struct {
    Value    string
    Occupied bool
    Deleted  bool
}

type Set struct {
    table    []SetEntry
    capacity int
    cnt      int
}

func NewSet(capacity int) *Set {
    if capacity < 8 {
        capacity = 8
    }

    return &Set{
        table:    make([]SetEntry, capacity),
        capacity: capacity,
        cnt:      0,
    }
}

func (s *Set) hashFunc(value string) int {
    var h uint64

    for i := 0; i < len(value); i++ {
        h = (h*31 + uint64(value[i])) % uint64(s.capacity)
    }

    return int(h)
}

func (s *Set) Has(value string) bool {
    start := s.hashFunc(value)

    for step := 0; step < s.capacity; step++ {
        i := (start + step) % s.capacity

        entry := s.table[i]

        if !entry.Occupied && !entry.Deleted {
            return false
        }

        if entry.Occupied && entry.Value == value {
            return true
        }
    }

    return false
}

func (s *Set) Rehash() {
    oldTable := s.table
    oldCapacity := s.capacity

    s.capacity *= 2
    s.table = make([]SetEntry, s.capacity)
    s.cnt = 0

    for i := 0; i < oldCapacity; i++ {
        if oldTable[i].Occupied {
            s.Add(oldTable[i].Value)
        }
    }
}

func (s *Set) Add(value string) {
    if (s.cnt+1)*10 >= s.capacity*7 {
        s.Rehash()
    }

    start := s.hashFunc(value)
    firstDeleted := -1

    for step := 0; step < s.capacity; step++ {
        i := (start + step) % s.capacity

        entry := s.table[i]

        if entry.Occupied && entry.Value == value {
            return
        }

        if !entry.Occupied && entry.Deleted && firstDeleted < 0 {
            firstDeleted = i
        }

        if !entry.Occupied && !entry.Deleted {
            if firstDeleted >= 0 {
                i = firstDeleted
            }

            s.table[i] = SetEntry{
                Value:    value,
                Occupied: true,
                Deleted:  false,
            }

            s.cnt++
            return
        }
    }

    if firstDeleted >= 0 {
        s.table[firstDeleted] = SetEntry{
            Value:    value,
            Occupied: true,
            Deleted:  false,
        }

        s.cnt++
    }
}

func (s *Set) Del(value string) {
    start := s.hashFunc(value)

    for step := 0; step < s.capacity; step++ {
        i := (start + step) % s.capacity

        entry := s.table[i]

        if !entry.Occupied && !entry.Deleted {
            return
        }

        if entry.Occupied && entry.Value == value {
            s.table[i].Occupied = false
            s.table[i].Deleted = true
            s.table[i].Value = ""

            s.cnt--
            return
        }
    }
}

func (s *Set) Size() int {
    return s.cnt
}

func (s *Set) GetAt(index int) string {
    if index < 0 {
        return ""
    }

    seen := 0

    for i := 0; i < s.capacity; i++ {
        if s.table[i].Occupied {
            if seen == index {
                return s.table[i].Value
            }

            seen++
        }
    }

    return ""
}

func (s *Set) Print() {
    fmt.Printf("Set size=%d: ", s.cnt)

    for i := 0; i < s.cnt; i++ {
        fmt.Print(s.GetAt(i), " ")
    }

    fmt.Println()
}

func (s *Set) Execute(cmd string, tokens structures.TokenList) {
    switch cmd {
    case "SETADD":
        if len(tokens) < 2 {
            fmt.Println("SETADD: missing value")
            return
        }

        s.Add(tokens[1])

    case "SETDEL":
        if len(tokens) < 2 {
            fmt.Println("SETDEL: missing value")
            return
        }

        s.Del(tokens[1])

    case "SET_AT":
        if len(tokens) < 2 {
            fmt.Println("SET_AT: missing value")
            return
        }

        if s.Has(tokens[1]) {
            fmt.Println("TRUE")
        } else {
            fmt.Println("FALSE")
        }
    }
}

func (s *Set) Save() string {
    result := fmt.Sprint(s.cnt)

    for i := 0; i < s.cnt; i++ {
        result += " " + s.GetAt(i)
    }

    return result
}

func (s *Set) Load(data string) {
    s.table = make([]SetEntry, s.capacity)
    s.cnt = 0

    fields := splitFields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 1; i < len(fields) && i <= count; i++ {
        s.Add(fields[i])
    }
}

func (s *Set) GetType() byte {
    return '2'
}