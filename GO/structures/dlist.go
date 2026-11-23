package structures

import "fmt"

type DList struct {
    head *DNode
    tail *DNode
}

func NewDList() *DList {
    return &DList{}
}

func (l *DList) InsertHead(value string) {
    newNode := NewDNode(value, nil, l.head)

    if l.head != nil {
        l.head.Prev = newNode
    } else {
        l.tail = newNode
    }

    l.head = newNode
}

func (l *DList) InsertTail(value string) {
    newNode := NewDNode(value, l.tail, nil)

    if l.tail != nil {
        l.tail.Next = newNode
    } else {
        l.head = newNode
    }

    l.tail = newNode
}

func (l *DList) InsertAfter(target, value string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current == nil {
        return
    }

    newNode := NewDNode(value, current, current.Next)

    if current.Next != nil {
        current.Next.Prev = newNode
    } else {
        l.tail = newNode
    }

    current.Next = newNode
}

func (l *DList) InsertBefore(target, value string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current == nil {
        return
    }

    newNode := NewDNode(value, current.Prev, current)

    if current.Prev != nil {
        current.Prev.Next = newNode
    } else {
        l.head = newNode
    }

    current.Prev = newNode
}

func (l *DList) Remove(value string) {
    current := l.head

    for current != nil && current.Data != value {
        current = current.Next
    }

    if current != nil {
        l.RemoveNode(current)
    }
}

func (l *DList) RemoveAfter(target string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current != nil && current.Next != nil {
        l.RemoveNode(current.Next)
    }
}

func (l *DList) RemoveBefore(target string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current != nil && current.Prev != nil {
        l.RemoveNode(current.Prev)
    }
}

func (l *DList) RemoveNode(node *DNode) {
    if node == nil {
        return
    }

    if node.Prev != nil {
        node.Prev.Next = node.Next
    } else {
        l.head = node.Next
    }

    if node.Next != nil {
        node.Next.Prev = node.Prev
    } else {
        l.tail = node.Prev
    }
}

func (l *DList) RemoveTail() string {
    if l.tail == nil {
        return "EMPTY"
    }

    value := l.tail.Data

    l.RemoveNode(l.tail)

    return value
}

func (l *DList) Contains(value string) bool {
    for current := l.head; current != nil; current = current.Next {
        if current.Data == value {
            return true
        }
    }

    return false
}

func (l *DList) FindNodeByPrefix(prefix string) *DNode {
    for current := l.head; current != nil; current = current.Next {
        if len(current.Data) >= len(prefix) &&
            current.Data[:len(prefix)] == prefix {
            return current
        }
    }

    return nil
}

func (l *DList) CollectByPrefix(prefix string, out *DList) {
    for current := l.head; current != nil; current = current.Next {
        if len(current.Data) >= len(prefix) &&
            current.Data[:len(prefix)] == prefix {
            out.InsertTail(current.Data)
        }
    }
}

func (l *DList) GetHead() *DNode {
    return l.head
}

func (l *DList) GetTail() *DNode {
    return l.tail
}

func (l *DList) GetSize() int {
    count := 0

    for current := l.head; current != nil; current = current.Next {
        count++
    }

    return count
}

func (l *DList) Clear() {
    l.head = nil
    l.tail = nil
}

func (l *DList) Print() {
    current := l.head

    fmt.Print("DList: ")

    for current != nil {
        fmt.Print(current.Data, " <-> ")
        current = current.Next
    }

    fmt.Println("nullptr")
}

func (l *DList) Execute(cmd string, tokens TokenList) {
    if cmd == "LPUSH" {
        if len(tokens) < 4 {
            fmt.Println("LPUSH: missing args")
            return
        }

        mode := tokens[2]
        value := tokens[3]

        switch mode {
        case "HEAD":
            l.InsertHead(value)

        case "TAIL":
            l.InsertTail(value)

        case "AFTER":
            if len(tokens) >= 5 {
                l.InsertAfter(value, tokens[4])
            }

        case "BEFORE":
            if len(tokens) >= 5 {
                l.InsertBefore(value, tokens[4])
            }
        }

    } else if cmd == "LDEL" {
        if len(tokens) < 3 {
            fmt.Println("LDEL: missing value")
            return
        }

        if len(tokens) >= 4 {
            mode := tokens[2]
            value := tokens[3]

            switch mode {
            case "AFTER":
                l.RemoveAfter(value)

            case "BEFORE":
                l.RemoveBefore(value)

            default:
                l.Remove(tokens[2])
            }

        } else {
            l.Remove(tokens[2])
        }
    }
}

func (l *DList) Save() string {
    count := 0
    result := ""

    for current := l.head; current != nil; current = current.Next {
        count++

        if result != "" {
            result += " "
        }

        result += current.Data
    }

    return fmt.Sprintf("%d %s", count, result)
}

func (l *DList) Load(data string) {
    l.Clear()

    fields := splitFields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 1; i < len(fields) && i <= count; i++ {
        l.InsertTail(fields[i])
    }
}

func (l *DList) GetType() byte {
    return 'L'
}