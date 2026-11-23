package lab2_tasks

import (
    "fmt"
    "strings"

    "lab2_go/structures"
)

type LogicEval struct {
    vals *structures.Stack
    ops  *structures.Stack
}

func NewLogicEval() *LogicEval {
    return &LogicEval{
        vals: structures.NewStack(),
        ops:  structures.NewStack(),
    }
}

func (l *LogicEval) prec(op byte) int {
    switch op {
    case '!':
        return 3
    case '&':
        return 2
    case '|', '^':
        return 1
    }

    return 0
}

func (l *LogicEval) applyOp() {
    opString := l.ops.Remove()

    if opString == "EMPTY" {
        return
    }

    op := opString[0]

    if op == '!' {
        a := l.vals.Remove()

        if a == "EMPTY" {
            l.vals.Insert("ERROR")
            return
        }

        if a == "0" {
            l.vals.Insert("1")
        } else {
            l.vals.Insert("0")
        }

    } else {
        b := l.vals.Remove()
        a := l.vals.Remove()

        if a == "EMPTY" || b == "EMPTY" {
            l.vals.Insert("ERROR")
            return
        }

        var x, y int

        fmt.Sscan(a, &x)
        fmt.Sscan(b, &y)

        result := 0

        switch op {
        case '&':
            result = x & y
        case '|':
            result = x | y
        case '^':
            result = x ^ y
        }

        l.vals.Insert(fmt.Sprint(result))
    }
}

func (l *LogicEval) clearStacks() {
    for l.vals.Remove() != "EMPTY" {
    }

    for l.ops.Remove() != "EMPTY" {
    }
}

func (l *LogicEval) Execute(cmd string, tokens structures.TokenList) {
    if cmd != "LOGIC_EVAL" {
        return
    }

    if len(tokens) < 2 {
        fmt.Println("LOGIC_EVAL: missing expression")
        return
    }

    l.clearStacks()

    expression := strings.Join(tokens[1:], " ")

    expectOperand := true
    valid := true

    for i := 0; i < len(expression) && valid; i++ {
        c := expression[i]

        if c == ' ' || c == '\t' {
            continue
        }

        if c == '0' || c == '1' {
            if !expectOperand {
                valid = false
                break
            }

            l.vals.Insert(string(c))
            expectOperand = false

            for {
                top := l.ops.Remove()

                if top == "EMPTY" {
                    break
                }

                if top == "!" {
                    l.ops.Insert(top)
                    l.applyOp()
                } else {
                    l.ops.Insert(top)
                    break
                }
            }

        } else if c == '(' {
            if !expectOperand {
                valid = false
                break
            }

            l.ops.Insert("(")
            expectOperand = true

        } else if c == ')' {
            if expectOperand {
                valid = false
                break
            }

            foundOpen := false

            for {
                top := l.ops.Remove()

                if top == "EMPTY" {
                    break
                }

                if top == "(" {
                    foundOpen = true
                    break
                }

                l.ops.Insert(top)
                l.applyOp()
            }

            if !foundOpen {
                valid = false
                break
            }

            for {
                top := l.ops.Remove()

                if top == "EMPTY" {
                    break
                }

                if top == "!" {
                    l.ops.Insert(top)
                    l.applyOp()
                } else {
                    l.ops.Insert(top)
                    break
                }
            }

            expectOperand = false

        } else if c == '!' || c == '&' || c == '|' || c == '^' {
            if c != '!' && expectOperand {
                valid = false
                break
            }

            if c == '!' && !expectOperand {
                valid = false
                break
            }

            for {
                top := l.ops.Remove()

                if top == "EMPTY" || top == "(" {
                    if top == "(" {
                        l.ops.Insert(top)
                    }

                    break
                }

                topPriority := l.prec(top[0])
                currentPriority := l.prec(c)

                if topPriority > currentPriority ||
                    (topPriority == currentPriority && c != '!') {

                    l.ops.Insert(top)
                    l.applyOp()

                } else {
                    l.ops.Insert(top)
                    break
                }
            }

            l.ops.Insert(string(c))
            expectOperand = true

        } else {
            valid = false
        }
    }

    if expectOperand {
        valid = false
    }

    for valid {
        top := l.ops.Remove()

        if top == "EMPTY" {
            break
        }

        if top == "(" {
            valid = false
            break
        }

        l.ops.Insert(top)
        l.applyOp()
    }

    result := l.vals.Remove()

    if l.vals.Remove() != "EMPTY" ||
        result == "EMPTY" ||
        result == "ERROR" {

        valid = false
    }

    if valid {
        fmt.Println("Result:", result)
    } else {
        fmt.Println("Incorrect expression")
    }
}

func (l *LogicEval) Print() {
    fmt.Println("Logic evaluator ready")
}

func (l *LogicEval) Save() string {
    return "LOGIC"
}

func (l *LogicEval) Load(data string) {
}

func (l *LogicEval) GetType() byte {
    return '1'
}