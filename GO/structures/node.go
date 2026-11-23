package structures

type Node struct {
    Data string
    Next *Node
}

type DNode struct {
    Data string
    Prev *DNode
    Next *DNode
}

type TNode struct {
    Data  int
    Left  *TNode
    Right *TNode
}

func NewNode(data string, next *Node) *Node {
    return &Node{
        Data: data,
        Next: next,
    }
}

func NewDNode(data string, prev, next *DNode) *DNode {
    return &DNode{
        Data: data,
        Prev: prev,
        Next: next,
    }
}

func NewTNode(value int) *TNode {
    return &TNode{
        Data: value,
    }
}