package structures

import "fmt"

type Queue struct {
    head *Node
    tail *Node
}

func NewQueue() *Queue {
    return &Queue{}
}

func (q *Queue) Insert(value string) {
    newNode := NewNode(value, nil)

    if q.tail != nil {
        q.tail.Next = newNode
    } else {
        q.head = newNode
    }

    q.tail = newNode
}

func (q *Queue) Remove() string {
    if q.head == nil {
        return "EMPTY"
    }

    value := q.head.Data

    q.head = q.head.Next

    if q.head == nil {
        q.tail = nil
    }

    return value
}

func (q *Queue) Print() {
    current := q.head

    fmt.Print("Queue (front -> back): ")

    for current != nil {
        fmt.Print(current.Data, " ")
        current = current.Next
    }

    fmt.Println()
}

func (q *Queue) Execute(cmd string, tokens TokenList) {
    if cmd == "QPUSH" {
        if len(tokens) < 3 {
            fmt.Println("QPUSH: missing value")
            return
        }

        q.Insert(tokens[2])

    } else if cmd == "QPOP" {
        fmt.Println(q.Remove())
    }
}

func (q *Queue) Save() string {
    count := 0
    current := q.head

    result := ""

    for current != nil {
        count++
        if result != "" {
            result += " "
        }
        result += current.Data

        current = current.Next
    }

    return fmt.Sprintf("%d %s", count, result)
}

func (q *Queue) Load(data string) {
    q.head = nil
    q.tail = nil

    fields := splitFields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 1; i < len(fields) && i <= count; i++ {
        q.Insert(fields[i])
    }
}

func (q *Queue) GetType() byte {
    return 'Q'
}