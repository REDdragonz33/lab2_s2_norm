package structures

import (
    "fmt"
    "strconv"
)

type CompleteTree struct {
    root      *TNode
    nodeCount int
}

func NewCompleteTree() *CompleteTree {
    return &CompleteTree{}
}

func destroyTree(node *TNode) {
    if node == nil {
        return
    }

    destroyTree(node.Left)
    destroyTree(node.Right)
}

func (t *CompleteTree) Insert(value int) {
    newNode := NewTNode(value)

    if t.root == nil {
        t.root = newNode
        t.nodeCount++

        fmt.Println("Inserted", value)
        return
    }

    t.nodeCount++

    target := t.nodeCount

    var bits []int

    for target > 1 {
        bits = append(bits, target%2)
        target /= 2
    }

    current := t.root

    for i := len(bits) - 1; i >= 0; i-- {
        if i == 0 {
            if bits[i] == 0 {
                current.Left = newNode
            } else {
                current.Right = newNode
            }
        } else {
            if bits[i] == 0 {
                current = current.Left
            } else {
                current = current.Right
            }
        }
    }

    fmt.Println("Inserted", value)
}

func searchRec(node *TNode, value int) bool {
    if node == nil {
        return false
    }

    if node.Data == value {
        return true
    }

    return searchRec(node.Left, value) ||
        searchRec(node.Right, value)
}

func (t *CompleteTree) Search(value int) bool {
    return searchRec(t.root, value)
}

func isCompleteRec(node *TNode, index, total int) bool {
    if node == nil {
        return true
    }

    if index >= total {
        return false
    }

    return isCompleteRec(
        node.Left,
        2*index+1,
        total,
    ) && isCompleteRec(
        node.Right,
        2*index+2,
        total,
    )
}

func (t *CompleteTree) Check() {
    if isCompleteRec(t.root, 0, t.nodeCount) {
        fmt.Println("TRUE")
    } else {
        fmt.Println("FALSE")
    }
}

func (t *CompleteTree) Print() {
    fmt.Print("Tree (levels): ")

    if t.root == nil {
        fmt.Println()
        return
    }

    queue := []*TNode{t.root}

    for len(queue) > 0 {
        current := queue[0]
        queue = queue[1:]

        fmt.Print(current.Data, " ")

        if current.Left != nil {
            queue = append(queue, current.Left)
        }

        if current.Right != nil {
            queue = append(queue, current.Right)
        }
    }

    fmt.Println()
}

func (t *CompleteTree) Save() string {
    result := strconv.Itoa(t.nodeCount)

    if t.root == nil {
        return result
    }

    queue := []*TNode{t.root}

    for len(queue) > 0 {
        current := queue[0]
        queue = queue[1:]

        result += " " + strconv.Itoa(current.Data)

        if current.Left != nil {
            queue = append(queue, current.Left)
        }

        if current.Right != nil {
            queue = append(queue, current.Right)
        }
    }

    return result
}

func (t *CompleteTree) Load(data string) {
    destroyTree(t.root)

    t.root = nil
    t.nodeCount = 0

    fields := splitFields(data)

    if len(fields) == 0 {
        return
    }

    var count int
    fmt.Sscan(fields[0], &count)

    for i := 1; i < len(fields) && i <= count; i++ {
        var value int
        fmt.Sscan(fields[i], &value)

        t.Insert(value)
    }
}

func (t *CompleteTree) Execute(cmd string, tokens TokenList) {
    switch cmd {
    case "TINSERT":
        if len(tokens) < 3 {
            fmt.Println("TINSERT: missing value")
            return
        }

        var value int
        fmt.Sscan(tokens[2], &value)

        t.Insert(value)

    case "TGET":
        if len(tokens) < 3 {
            fmt.Println("TGET: missing value")
            return
        }

        var value int
        fmt.Sscan(tokens[2], &value)

        if t.Search(value) {
            fmt.Println("TRUE")
        } else {
            fmt.Println("FALSE")
        }

    case "TCHECK":
        t.Check()
    }
}

func (t *CompleteTree) GetType() byte {
    return 'T'
}