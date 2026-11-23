package structures

import "fmt"

type Array struct {
    data     []string
    size     int
    capacity int
}

func NewArray() *Array {
    return &Array{
        data:     nil,
        size:     0,
        capacity: 0,
    }
}

func (a *Array) Resize(newCapacity int) {
    newData := make([]string, newCapacity)

    copy(newData, a.data)

    a.data = newData
    a.capacity = newCapacity
}

func (a *Array) InsertEnd(value string) {
    if a.size == a.capacity {
        newCapacity := 1

        if a.capacity != 0 {
            newCapacity = a.capacity * 2
        }

        a.Resize(newCapacity)
    }

    a.data[a.size] = value
    a.size++
}

func (a *Array) InsertAt(index int, value string) {
    if index < 0 || index > a.size {
        fmt.Println("Index out of range")
        return
    }

    if a.size == a.capacity {
        newCapacity := 1

        if a.capacity != 0 {
            newCapacity = a.capacity * 2
        }

        a.Resize(newCapacity)
    }

    a.data = append(a.data, "")

    copy(
        a.data[index+1:],
        a.data[index:a.size],
    )

    a.data[index] = value
    a.size++
}

func (a *Array) RemoveAt(index int) {
    if index < 0 || index >= a.size {
        fmt.Println("Index out of range")
        return
    }

    copy(
        a.data[index:],
        a.data[index+1:a.size],
    )

    a.size--
    a.data = a.data[:a.size]
}

func (a *Array) Get(index int) string {
    if index < 0 || index >= a.size {
        return "ERROR"
    }

    return a.data[index]
}

func (a *Array) Set(index int, value string) {
    if index < 0 || index >= a.size {
        fmt.Println("Index out of range")
        return
    }

    a.data[index] = value
}

func (a *Array) Length() int {
    return a.size
}

func (a *Array) Print() {
    fmt.Printf("Array[%d]: ", a.size)

    for i := 0; i < a.size; i++ {
        fmt.Print(a.data[i], " ")
    }

    fmt.Println()
}

func (a *Array) Execute(cmd string, tokens TokenList) {
    switch cmd {
    case "MPUSH":
        if len(tokens) == 3 {
            a.InsertEnd(tokens[2])
        } else if len(tokens) == 4 {
            var index int
            fmt.Sscan(tokens[2], &index)
            a.InsertAt(index, tokens[3])
        }

    case "MDEL":
        if len(tokens) < 3 {
            fmt.Println("MDEL: missing index")
            return
        }

        var index int
        fmt.Sscan(tokens[2], &index)

        a.RemoveAt(index)

    case "MGET":
        if len(tokens) < 3 {
            fmt.Println("MGET: missing index")
            return
        }

        var index int
        fmt.Sscan(tokens[2], &index)

        fmt.Println(a.Get(index))

    case "MSET":
        if len(tokens) < 4 {
            fmt.Println("MSET: missing args")
            return
        }

        var index int
        fmt.Sscan(tokens[2], &index)

        a.Set(index, tokens[3])

    case "MLEN":
        fmt.Println(a.Length())
    }
}

func (a *Array) Save() string {
    result := fmt.Sprint(a.size)

    for i := 0; i < a.size; i++ {
        result += " " + a.data[i]
    }

    return result
}

func (a *Array) Load(data string) {
    a.data = nil
    a.size = 0
    a.capacity = 0

    fields := splitFields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 1; i < len(fields) && i <= count; i++ {
        a.InsertEnd(fields[i])
    }
}

func (a *Array) GetType() byte {
    return 'M'
}