package structures

import "fmt"

type SList struct {
    head *Node
    tail *Node
}

func NewSList() *SList {
    return &SList{}
}

func (l *SList) InsertHead(value string) {
    newNode := NewNode(value, l.head)

    l.head = newNode

    if l.tail == nil {
        l.tail = newNode
    }
}

func (l *SList) InsertTail(value string) {
    newNode := NewNode(value, nil)

    if l.tail == nil {
        l.head = newNode
    } else {
        l.tail.Next = newNode
    }

    l.tail = newNode
}

func (l *SList) InsertAfter(target, value string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current == nil {
        return
    }

    newNode := NewNode(value, current.Next)
    current.Next = newNode

    if current == l.tail {
        l.tail = newNode
    }
}

func (l *SList) InsertBefore(target, value string) {
    if l.head == nil {
        return
    }

    if l.head.Data == target {
        l.InsertHead(value)
        return
    }

    current := l.head

    for current.Next != nil && current.Next.Data != target {
        current = current.Next
    }

    if current.Next == nil {
        return
    }

    newNode := NewNode(value, current.Next)
    current.Next = newNode
}

func (l *SList) Remove(value string) {
    if l.head == nil {
        return
    }

    if l.head.Data == value {
        l.head = l.head.Next

        if l.head == nil {
            l.tail = nil
        }

        return
    }

    current := l.head

    for current.Next != nil && current.Next.Data != value {
        current = current.Next
    }

    if current.Next == nil {
        return
    }

    if current.Next == l.tail {
        l.tail = current
    }

    current.Next = current.Next.Next
}

func (l *SList) RemoveAfter(target string) {
    current := l.head

    for current != nil && current.Data != target {
        current = current.Next
    }

    if current == nil || current.Next == nil {
        return
    }

    current.Next = current.Next.Next

    if current.Next == nil {
        l.tail = current
    }
}

func (l *SList) RemoveBefore(target string) {
    if l.head == nil || l.head.Next == nil {
        return
    }

    if l.head.Next.Data == target {
        l.head = l.head.Next
        return
    }

    current := l.head

    for current.Next != nil &&
        current.Next.Next != nil {

        if current.Next.Next.Data == target {
            current.Next = current.Next.Next
            return
        }

        current = current.Next
    }
}

func (l *SList) Print() {
    current := l.head

    fmt.Print("SList: ")

    for current != nil {
        fmt.Print(current.Data, " -> ")
        current = current.Next
    }

    fmt.Println("nullptr")
}

func (l *SList) Execute(cmd string, tokens TokenList) {
    if cmd == "FPUSH" {
        if len(tokens) < 4 {
            fmt.Println("FPUSH: missing args")
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

    } else if cmd == "FDEL" {
        if len(tokens) < 3 {
            fmt.Println("FDEL: missing value")
            return
        }

        if len(tokens) >= 4 {
            mode := tokens[2]
            target := tokens[3]

            switch mode {
            case "AFTER":
                l.RemoveAfter(target)

            case "BEFORE":
                l.RemoveBefore(target)

            default:
                l.Remove(tokens[2])
            }

        } else {
            l.Remove(tokens[2])
        }

    } else if cmd == "FGET" {
        fmt.Println("FGET by index: not implemented")
    }
}

func (l *SList) Save() string {
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

func (l *SList) Load(data string) {
    l.head = nil
    l.tail = nil

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

func (l *SList) GetType() byte {
    return 'F'
}