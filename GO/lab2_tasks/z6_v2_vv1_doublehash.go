package lab2_tasks

import (
    "fmt"
    "strings"
    "time"

    "lab2_go/structures"
)

type HashTable struct {
    buckets []*structures.DList
    cap int

    chainCollisions int
    chainCount int

    cuckoo []string
    cuckooCap int
    cuckooCount int
    cuckooCollisions int
}

func NewHashTable(capacity int) *HashTable {
    if capacity < 8 {
        capacity = 8
    }

    h := &HashTable{
        cap: capacity,
        buckets: make([]*structures.DList, capacity),
        cuckooCap: 32,
        cuckoo: make([]string, 32),
    }

    for i := range h.buckets {
        h.buckets[i] = structures.NewDList()
    }

    return h
}

func (h *HashTable) h1(key string, size int) int {
    hash := 0

    for i := 0; i < len(key); i++ {
        hash = (hash*31 + int(key[i])) % size
    }

    return hash
}

func (h *HashTable) h2(key string, size int) int {
    hash := 0

    for i := 0; i < len(key); i++ {
        hash = (hash*131 + int(key[i])) % size
    }

    return hash
}

// Метод цепочек: вставка.
func (h *HashTable) ChainPut(key, value string) {
    index := h.h1(key, h.cap)

    old := h.buckets[index].FindNodeByPrefix(key + ":")

    if old != nil {
        h.buckets[index].RemoveNode(old)
    } else {
        if h.buckets[index].GetSize() > 0 {
            h.chainCollisions++
        }

        h.chainCount++
    }

    h.buckets[index].InsertTail(key + ":" + value)
}

// Метод цепочек: получение.
func (h *HashTable) ChainGet(key string) string {
    index := h.h1(key, h.cap)

    node := h.buckets[index].FindNodeByPrefix(key + ":")

    if node == nil {
        return "EMPTY"
    }

    return strings.TrimPrefix(node.Data, key+":")
}

// Метод цепочек: удаление.
func (h *HashTable) ChainDel(key string) {
    index := h.h1(key, h.cap)

    node := h.buckets[index].FindNodeByPrefix(key + ":")

    if node != nil {
        h.buckets[index].RemoveNode(node)
        h.chainCount--
    }
}

// Повторное хеширование кукушки.
func (h *HashTable) cuckooRehash() {
    old := h.cuckoo
    oldCap := h.cuckooCap

    h.cuckooCap *= 2
    h.cuckoo = make([]string, h.cuckooCap)
    h.cuckooCount = 0

    for i := 0; i < oldCap; i++ {
        if old[i] == "" {
            continue
        }

        parts := strings.SplitN(old[i], ":", 2)

        if len(parts) != 2 {
            continue
        }

        h.cuckooInsert(parts[0], parts[1], 0)
    }
}

// Вставка с вытеснением.
func (h *HashTable) cuckooInsert(
    key string,
    value string,
    depth int,
) bool {

    i1 := h.h1(key, h.cuckooCap)
    i2 := h.h2(key, h.cuckooCap)

    prefix := key + ":"

    if strings.HasPrefix(h.cuckoo[i1], prefix) {
        h.cuckoo[i1] = prefix + value
        return true
    }

    if strings.HasPrefix(h.cuckoo[i2], prefix) {
        h.cuckoo[i2] = prefix + value
        return true
    }

    current := key + ":" + value
    pos := i1

    maxKicks := h.cuckooCap * 2

    for kick := 0; kick < maxKicks; kick++ {

        if h.cuckoo[pos] == "" {
            h.cuckoo[pos] = current
            h.cuckooCount++

            return true
        }

        h.cuckooCollisions++

        evicted := h.cuckoo[pos]
        h.cuckoo[pos] = current

        parts := strings.SplitN(evicted, ":", 2)

        evictedKey := parts[0]

        current = evicted

        first := h.h1(evictedKey, h.cuckooCap)
        second := h.h2(evictedKey, h.cuckooCap)

        if pos == first {
            pos = second
        } else {
            pos = first
        }
    }

    parts := strings.SplitN(current, ":", 2)

    if len(parts) != 2 {
        return false
    }

    pendingKey := parts[0]
    pendingValue := parts[1]

    h.cuckooRehash()

    if depth > 8 {
        h.cuckooRehash()
    }

    return h.cuckooInsert(
        pendingKey,
        pendingValue,
        depth+1,
    )
}

// Публичная вставка кукушки.
func (h *HashTable) CuckooPut(key, value string) bool {
    return h.cuckooInsert(key, value, 0)
}

// Получение элемента кукушки.
func (h *HashTable) CuckooGet(key string) string {
    i1 := h.h1(key, h.cuckooCap)
    i2 := h.h2(key, h.cuckooCap)

    prefix := key + ":"

    if strings.HasPrefix(h.cuckoo[i1], prefix) {
        return strings.TrimPrefix(h.cuckoo[i1], prefix)
    }

    if strings.HasPrefix(h.cuckoo[i2], prefix) {
        return strings.TrimPrefix(h.cuckoo[i2], prefix)
    }

    return "EMPTY"
}

// Эмпирический анализ.
func (h *HashTable) Analyze() {
    fmt.Println("=== Chains ===")

    fmt.Printf(
        "Count=%d Collisions=%d\n",
        h.chainCount,
        h.chainCollisions,
    )

    fmt.Println(
        "Method: separate chaining (each bucket stores a DList).",
    )

    fmt.Println("Average: O(1 + load factor), worst: O(n).")

    fmt.Println(
        "Pros: simple insertion/deletion, load factor can exceed 1.",
    )

    fmt.Println(
        "Cons: extra memory for nodes, linear search inside a bucket.",
    )

    fmt.Println("=== Cuckoo ===")

    fmt.Printf(
        "Count=%d Collisions=%d\n",
        h.cuckooCount,
        h.cuckooCollisions,
    )

    fmt.Println(
        "Method: cuckoo hashing (a key can occupy one of two hash positions).",
    )

    fmt.Println(
        "Lookup: O(1), insertion: O(1) amortized, rehash: O(n).",
    )

    fmt.Println(
        "Pros: at most two positions are checked during lookup.",
    )

    fmt.Println(
        "Cons: insertion can trigger rehashing; deletion needs tombstones or markers.",
    )

    const testCount = 1000

    chainTest := NewHashTable(64)
    cuckooTest := NewHashTable(64)

    startChain := time.Now()

    for i := 0; i < testCount; i++ {
        key := fmt.Sprintf("test_key_%d", i)
        chainTest.ChainPut(key, "value")
    }

    for i := 0; i < testCount; i++ {
        key := fmt.Sprintf("test_key_%d", i)
        chainTest.ChainGet(key)
    }

    chainTime := time.Since(startChain).Microseconds()

    startCuckoo := time.Now()

    for i := 0; i < testCount; i++ {
        key := fmt.Sprintf("test_key_%d", i)
        cuckooTest.CuckooPut(key, "value")
    }

    for i := 0; i < testCount; i++ {
        key := fmt.Sprintf("test_key_%d", i)
        cuckooTest.CuckooGet(key)
    }

    cuckooTime := time.Since(startCuckoo).Microseconds()

    fmt.Printf(
        "=== Empirical test (%d insertions + %d lookups) ===\n",
        testCount,
        testCount,
    )

    fmt.Printf(
        "Chains: %d us, collisions=%d\n",
        chainTime,
        chainTest.chainCollisions,
    )

    fmt.Printf(
        "Cuckoo: %d us, collisions=%d\n",
        cuckooTime,
        cuckooTest.cuckooCollisions,
    )

    fmt.Println(
        "Note: time depends on the computer and current system load.",
    )
}

// Сбор элементов по префиксу.
func (h *HashTable) CollectEdges(
    prefix string,
    out *structures.DList,
) {
    for i := 0; i < h.cap; i++ {
        h.buckets[i].CollectByPrefix(prefix, out)
    }
}

func (h *HashTable) Print() {
    fmt.Println("HashTable ready")
}

func (h *HashTable) Execute(
    cmd string,
    tokens structures.TokenList,
) {
    switch cmd {
    case "HT_PUT":
        if len(tokens) < 3 {
            return
        }

        h.ChainPut(tokens[1], tokens[2])

    case "HT_GET":
        if len(tokens) < 2 {
            return
        }

        fmt.Println(h.ChainGet(tokens[1]))

    case "HT_DEL":
        if len(tokens) < 2 {
            return
        }

        h.ChainDel(tokens[1])

    case "CK_PUT":
        if len(tokens) < 3 {
            return
        }

        h.CuckooPut(tokens[1], tokens[2])

    case "CK_GET":
        if len(tokens) < 2 {
            return
        }

        fmt.Println(h.CuckooGet(tokens[1]))

    case "HT_ANALYZE":
        h.Analyze()
    }
}

func (h *HashTable) Save() string {
    return fmt.Sprintf("HT %d", h.chainCount)
}

func (h *HashTable) Load(data string) {
    // Формат сохранения: HT <количество>.
}

func (h *HashTable) GetType() byte {
    return '6'
}