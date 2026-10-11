package parser

import (
	"fmt"
	"sort"
	"strconv"
)

type jsonType int

const (
	jsonTypeNil jsonType = iota
	jsonTypeObject
	jsonTypeArray
	jsonTypeStr
	jsonTypeNum
	jsonTypeBool
)

type JsonNode struct {
	val      any
	nodeType jsonType
}

func NewObjectNode() JsonNode {
	return JsonNode{val: make(map[string]JsonNode), nodeType: jsonTypeObject}
}

func NewArrayNode() JsonNode {
	return JsonNode{val: []JsonNode{}, nodeType: jsonTypeArray}
}

func NewStrNode(v string) JsonNode {
	return JsonNode{val: v, nodeType: jsonTypeStr}
}

func NewNumNode(v float64) JsonNode {
	return JsonNode{val: v, nodeType: jsonTypeNum}
}

func NewBoolNode(v bool) JsonNode {
	return JsonNode{val: v, nodeType: jsonTypeBool}
}

func NewNode() JsonNode {
	return JsonNode{nodeType: jsonTypeNil}
}

func (n *JsonNode) Get() *any {
	return &n.val
}

func (n *JsonNode) String(sorted bool) string {
	switch n.nodeType {
	case jsonTypeNil:
		return "null"
	case jsonTypeObject:
		objVal, ok := n.val.(map[string]JsonNode)
		if !ok {
			panic(fmt.Sprintf("invalid type %T", n.val))
		}
		objStr := ""
		keys := getKeys(objVal, sorted)
		for _, key := range keys {
			objStr = objStr + fmt.Sprintf("%q", key) + ":"
			value := objVal[key]
			objStr = objStr + value.String(sorted) + ","
		}
		if len(objStr) > 0 {
			objStr = objStr[:len(objStr)-1]
		}
		return fmt.Sprintf("{%s}", objStr)
	case jsonTypeArray:
		arrayVal, ok := n.val.([]JsonNode)
		if !ok {
			panic(fmt.Sprintf("invalid type %T", n.val))
		}
		arrayStr := ""
		for _, val := range arrayVal {
			next := val.String(sorted)
			arrayStr = arrayStr + next + ","
		}
		if len(arrayStr) > 0 {
			arrayStr = arrayStr[:len(arrayStr)-1]
		}
		return fmt.Sprintf("[%s]", arrayStr)
	case jsonTypeStr:
		strVal, ok := n.val.(string)
		if !ok {
			panic(fmt.Sprintf("invalid type %T", n.val))
		}
		return fmt.Sprintf("\"%s\"", strVal)
	case jsonTypeNum:
		numVal, ok := n.val.(float64)
		if !ok {
			panic(fmt.Sprintf("invalid type %T", n.val))
		}
		return strconv.FormatFloat(numVal, 'f', -1, 64)
	case jsonTypeBool:
		boolVal, ok := n.val.(bool)
		if !ok {
			panic(fmt.Sprintf("invalid type %T", n.val))
		}
		if boolVal {
			return "true"
		} else {
			return "false"
		}
	default:
		panic(fmt.Sprintf("not supported jnode type %v", n.nodeType))
	}
}

func (n *JsonNode) GetObj() map[string]JsonNode {
	objVal, ok := n.val.(map[string]JsonNode)
	if !ok {
		panic(fmt.Sprintf("invalid type %T", n.val))
	}
	return objVal
}

func getKeys(kvps map[string]JsonNode, sorted bool) []string {
	keys := make([]string, len(kvps))

	i := 0
	for key := range kvps {
		keys[i] = key
		i++
	}

	if sorted {
		sort.Strings(keys)
	}

	return keys
}
