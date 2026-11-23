package structures

import "fmt"

type Stack struct {
    head *Node
}

func NewStack() *Stack {
    return &Stack{
        head: nil,
    }
}

func (s *Stack) Insert(value string) {
    newNode := NewNode(value, s.head)
    s.head = newNode
}

func (s *Stack) Remove() string {
    if s.head == nil {
        return "EMPTY"
    }

    value := s.head.Data
    s.head = s.head.Next

    return value
}

func (s *Stack) Print() {
    current := s.head

    fmt.Print("Stack (top -> bottom): ")

    for current != nil {
        fmt.Print(current.Data, " ")
        current = current.Next
    }

    fmt.Println()
}

func (s *Stack) Execute(cmd string, tokens TokenList) {
    if cmd == "SPUSH" {
        if len(tokens) < 3 {
            fmt.Println("SPUSH: missing value")
            return
        }

        s.Insert(tokens[2])

    } else if cmd == "SPOP" {
        fmt.Println(s.Remove())
    }
}

func (s *Stack) Save() string {
    count := 0
    current := s.head

    for current != nil {
        count++
        current = current.Next
    }

    result := fmt.Sprint(count)

    current = s.head

    for current != nil {
        result += " " + current.Data
        current = current.Next
    }

    return result
}

func (s *Stack) Load(data string) {
    s.head = nil

    var count int
    fmt.Sscan(data, &count)

    if count <= 0 {
        return
    }

    var values []string
    fields := splitFields(data)

    for i := 1; i < len(fields) && len(values) < count; i++ {
        values = append(values, fields[i])
    }

    for i := len(values) - 1; i >= 0; i-- {
        s.Insert(values[i])
    }
}

func (s *Stack) GetType() byte {
    return 'S'
}