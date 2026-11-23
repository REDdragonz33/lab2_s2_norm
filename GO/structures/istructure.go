package structures

type TokenList []string

type IStructure interface {
    Print()
    Execute(cmd string, tokens TokenList)
    Save() string
    Load(data string)
    GetType() byte
}