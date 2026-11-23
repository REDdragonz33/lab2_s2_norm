package lab2_tasks

import (
    "fmt"
    "sort"

    "lab2_go/structures"
)

type TreeForks struct {
    root *structures.TNode
}

func NewTreeForks() *TreeForks {
    return &TreeForks{
        root: nil,
    }
}

func (t *TreeForks) Destroy() {
    t.root = nil
}

func (t *TreeForks) Insert(value int) {
    if t.root == nil {
        t.root = &structures.TNode{
            Data: value,
        }

        return
    }

    current := t.root

    for {
        if value < current.Data {
            if current.Left == nil {
                current.Left = &structures.TNode{
                    Data: value,
                }

                return
            }

            current = current.Left

        } else if value > current.Data {
            if current.Right == nil {
                current.Right = &structures.TNode{
                    Data: value,
                }

                return
            }

            current = current.Right

        } else {
            return
        }
    }
}

func (t *TreeForks) PrintForks() {
    stack := []*structures.TNode{}
    result := []int{}

    current := t.root

    for current != nil || len(stack) > 0 {

        for current != nil {
            stack = append(stack, current)
            current = current.Left
        }

        current = stack[len(stack)-1]
        stack = stack[:len(stack)-1]

        if current.Left != nil && current.Right != nil {
            result = append(result, current.Data)
        }

        current = current.Right
    }

    sort.Ints(result)

    fmt.Print("Forks: ")

    for _, value := range result {
        fmt.Print(value, " ")
    }

    fmt.Println()
}

func (t *TreeForks) Print() {
    fmt.Println("TreeForks ready")
}

func (t *TreeForks) Execute(cmd string, tokens structures.TokenList) {
    if cmd != "TREE_FORKS" {
        return
    }

    t.Destroy()

    for i := 1; i < len(tokens); i++ {
        var value int

        fmt.Sscan(tokens[i], &value)

        if value == 0 {
            break
        }

        t.Insert(value)
    }

    t.PrintForks()
}

func (t *TreeForks) Save() string {
    return "FORKS"
}

func (t *TreeForks) Load(data string) {
    t.Destroy()
}

func (t *TreeForks) GetType() byte {
    return '5'
}