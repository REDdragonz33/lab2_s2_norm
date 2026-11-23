package lab2_tasks

import (
    "fmt"
    "strconv"
    "strings"

    "lab2_go/structures"
)

type Block struct {
    W int
    H int
}

type Pyramid struct {
    blocks *structures.Array
}

func NewPyramid() *Pyramid {
    return &Pyramid{
        blocks: structures.NewArray(),
    }
}

func (p *Pyramid) ReadBlock(index int) Block {
    value := p.blocks.Get(index)

    parts := strings.Split(value, ",")

    if len(parts) != 2 {
        return Block{}
    }

    w, _ := strconv.Atoi(parts[0])
    h, _ := strconv.Atoi(parts[1])

    return Block{W: w, H: h}
}

func (p *Pyramid) AddBlock(w, h int) {
    if w <= 0 || h <= 0 {
        fmt.Println("Block dimensions must be positive")
        return
    }

    value := strconv.Itoa(w) + "," + strconv.Itoa(h)

    p.blocks.InsertEnd(value)
}

func mergeSort(a, temp []Block, left, right int) {
    if right-left <= 1 {
        return
    }

    mid := left + (right-left)/2

    mergeSort(a, temp, left, mid)
    mergeSort(a, temp, mid, right)

    i := left
    j := mid
    k := left

    for i < mid && j < right {
        if a[i].W >= a[j].W {
            temp[k] = a[i]
            i++
        } else {
            temp[k] = a[j]
            j++
        }

        k++
    }

    for i < mid {
        temp[k] = a[i]
        i++
        k++
    }

    for j < right {
        temp[k] = a[j]
        j++
        k++
    }

    for i := left; i < right; i++ {
        a[i] = temp[i]
    }
}

func (p *Pyramid) SortDesc() {
    count := p.blocks.Length()

    if count < 2 {
        return
    }

    data := make([]Block, count)
    temp := make([]Block, count)

    for i := 0; i < count; i++ {
        data[i] = p.ReadBlock(i)
    }

    mergeSort(data, temp, 0, count)

    for i := 0; i < count; i++ {
        value := fmt.Sprintf("%d,%d", data[i].W, data[i].H)
        p.blocks.Set(i, value)
    }
}

func (p *Pyramid) CalcMax() int64 {
    count := p.blocks.Length()

    if count == 0 {
        return 0
    }

    p.SortDesc()

    var total int64

    i := 0

    for i < count {
        current := p.ReadBlock(i)

        width := current.W
        bestH := current.H

        j := i + 1

        for j < count {
            next := p.ReadBlock(j)

            if next.W != width {
                break
            }

            if next.H > bestH {
                bestH = next.H
            }

            j++
        }

        total += int64(bestH)
        i = j
    }

    return total
}

func (p *Pyramid) Print() {
    fmt.Println(
        "Pyramid ready, blocks=",
        p.blocks.Length(),
    )
}

func (p *Pyramid) Execute(cmd string, tokens structures.TokenList) {
    switch cmd {
    case "PIRAM_ADD":
        if len(tokens) < 3 {
            fmt.Println("PIRAM_ADD: missing args")
            return
        }

        var w, h int

        fmt.Sscan(tokens[1], &w)
        fmt.Sscan(tokens[2], &h)

        p.AddBlock(w, h)

    case "PIRAM_CALC":
        fmt.Println("Max height:", p.CalcMax())

    case "PIRAM_CLEAR":
        for p.blocks.Length() > 0 {
            p.blocks.RemoveAt(p.blocks.Length() - 1)
        }
    }
}

func (p *Pyramid) Save() string {
    count := p.blocks.Length()

    result := fmt.Sprint(count)

    for i := 0; i < count; i++ {
        block := p.ReadBlock(i)

        result += fmt.Sprintf(
            " %d %d",
            block.W,
            block.H,
        )
    }

    return result
}

func (p *Pyramid) Load(data string) {
    for p.blocks.Length() > 0 {
        p.blocks.RemoveAt(p.blocks.Length() - 1)
    }

    fields := strings.Fields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    pos := 1

    for i := 0; i < count && pos+1 < len(fields); i++ {
        var w, h int

        fmt.Sscan(fields[pos], &w)
        fmt.Sscan(fields[pos+1], &h)

        p.AddBlock(w, h)

        pos += 2
    }
}

func (p *Pyramid) GetType() byte {
    return '4'
}