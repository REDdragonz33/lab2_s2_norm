package lab2_tasks

import (
    "fmt"
    "strconv"
    "strings"

    "lab2_go/structures"
)

type LFUCache struct {
    list     *structures.DList
    capacity int
}

func NewLFUCache(capacity int) *LFUCache {
    if capacity < 0 {
        capacity = 0
    }

    return &LFUCache{
        list:     structures.NewDList(),
        capacity: capacity,
    }
}

// Увеличение частоты использования.
func (l *LFUCache) Bump(node *structures.DNode) {
    if node == nil {
        return
    }

    parts := strings.SplitN(node.Data, "|", 3)

    if len(parts) != 3 {
        return
    }

    key := parts[0]

    frequency, err := strconv.Atoi(parts[1])

    if err != nil {
        return
    }

    value := parts[2]

    if frequency < int(^uint(0)>>1) {
        frequency++
    }

    l.list.RemoveNode(node)

    l.list.InsertTail(
        fmt.Sprintf("%s|%d|%s", key, frequency, value),
    )
}

// Удаление элемента с минимальной частотой.
func (l *LFUCache) EvictMin() {
    current := l.list.GetHead()

    var minNode *structures.DNode

    minFrequency := int(^uint(0) >> 1)

    for current != nil {
        parts := strings.SplitN(current.Data, "|", 3)

        if len(parts) != 3 {
            current = current.Next
            continue
        }

        frequency, err := strconv.Atoi(parts[1])

        if err != nil {
            current = current.Next
            continue
        }

        if frequency < minFrequency {
            minFrequency = frequency
            minNode = current
        }

        current = current.Next
    }

    if minNode != nil {
        l.list.RemoveNode(minNode)
    }
}

// SET.
func (l *LFUCache) Set(key, value string) {
    if l.capacity <= 0 {
        return
    }

    node := l.list.FindNodeByPrefix(key + "|")

    if node != nil {
        parts := strings.SplitN(node.Data, "|", 3)

        if len(parts) != 3 {
            return
        }

        frequency := parts[1]

        l.list.RemoveNode(node)

        l.list.InsertTail(
            key + "|" + frequency + "|" + value,
        )

    } else {
        if l.list.GetSize() >= l.capacity {
            l.EvictMin()
        }

        l.list.InsertTail(
            key + "|1|" + value,
        )
    }
}

// GET.
func (l *LFUCache) Get(key string) string {
    node := l.list.FindNodeByPrefix(key + "|")

    if node == nil {
        return "-1"
    }

    parts := strings.SplitN(node.Data, "|", 3)

    if len(parts) != 3 {
        return "-1"
    }

    value := parts[2]

    l.Bump(node)

    return value
}

func (l *LFUCache) Print() {
    l.list.Print()
}

func (l *LFUCache) Execute(
    cmd string,
    tokens structures.TokenList,
) {
    switch cmd {
    case "LFU_SET":
        if len(tokens) < 3 {
            fmt.Println("LFU_SET: missing args")
            return
        }

        l.Set(tokens[1], tokens[2])

    case "LFU_GET":
        if len(tokens) < 2 {
            fmt.Println("LFU_GET: missing key")
            return
        }

        fmt.Println(l.Get(tokens[1]))

    case "LFU_CAP":
        if len(tokens) < 2 {
            fmt.Println("LFU_CAP: missing capacity")
            return
        }

        var capacity int

        fmt.Sscan(tokens[1], &capacity)

        if capacity < 0 {
            capacity = 0
        }

        l.capacity = capacity

        for l.list.GetSize() > l.capacity {
            l.EvictMin()
        }
    }
}

func (l *LFUCache) Save() string {
    return fmt.Sprintf(
        "%d %s",
        l.capacity,
        l.list.Save(),
    )
}

func (l *LFUCache) Load(data string) {
    l.list.Clear()

    fields := strings.Fields(data)

    if len(fields) == 0 {
        return
    }

    fmt.Sscan(fields[0], &l.capacity)

    if l.capacity < 0 {
        l.capacity = 0
    }

    if len(fields) < 2 {
        return
    }

    l.list.Load(strings.Join(fields[1:], " "))

    for l.list.GetSize() > l.capacity {
        l.EvictMin()
    }
}

func (l *LFUCache) GetType() byte {
    return '7'
}