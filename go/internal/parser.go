package parser

import (
	"fmt"
	"strconv"
	"strings"
)

type Parser struct {
	input string
	index int
}

func (p *Parser) Parse(input string) JsonNode {
	p.input = input
	p.index = 0
	return p.parse()
}

func NewParser() Parser {
	return Parser{input: "", index: 0}
}

func (p *Parser) get() byte {
	p.skip()
	return p.input[p.index]
}

func (p *Parser) readValue() string {
	val := ""
	for p.index < len(p.input) {
		v := p.input[p.index]
		if (v == ',') || (v == ']') || (v == '"') || (v == '}') {
			break
		}
		val += string(v)
		p.index++
	}
	if p.input[p.index] == '"' {
		p.index++
	}
	if p.input[p.index] == ']' {
		p.index--
	}
	return val
}

func (p *Parser) parse() JsonNode {
	if p.get() == '{' {
		return p.parseObject()
	}
	if p.get() == '[' {
		return p.parseArray()
	}
	if p.get() == '"' {
		return p.parseStr()
	}
	if isBool(p.get()) {
		return p.parseBool()
	}
	if isNumber(p.get()) {
		return p.parseNumber()
	}
	panic(fmt.Sprintf("not supported token %v", p.get()))
}

func (p *Parser) parseObject() JsonNode {
	p.index++

	node := NewObjectNode()

	mapVal := node.val.(map[string]JsonNode)

	for p.index < len(p.input) {
		if p.get() == '}' {
			break
		}
		p.skip()
		name := p.getFieldName()
		_, exists := mapVal[name]
		if exists {
			panic(fmt.Sprintf("duplicate field %v", name))
		}
		if p.get() != ':' {
			panic(fmt.Sprintf("invalid kvp separator in %s", name))
		}
		p.index++
		mapVal[name] = p.parse()
		p.index++
	}

	return node
}

func (p *Parser) parseArray() JsonNode {
	p.index++

	node := NewArrayNode()

	arrVal := node.val.([]JsonNode)

	for p.index < len(p.input) {
		if p.get() == ']' {
			break
		}
		next := p.parse()
		arrVal = append(arrVal, next)
		p.index++
	}

	node.val = arrVal

	p.index++

	return node
}

func (p *Parser) parseStr() JsonNode {
	p.index++

	val := p.readValue()

	return NewStrNode(val)
}

func (p *Parser) parseBool() JsonNode {
	val := strings.TrimSpace(p.readValue())
	if strings.EqualFold(val, "true") {
		return NewBoolNode(true)
	}
	if strings.EqualFold(val, "false") {
		return NewBoolNode(false)
	}

	panic(fmt.Sprintf("not supported bool value %v", val))
}

func (p *Parser) parseNumber() JsonNode {
	sign := 1
	if p.get() == '-' {
		sign = -1
		p.index++
	}

	valStr := strings.TrimSpace(p.readValue())

	val, err := strconv.ParseFloat(valStr, 64)
	if err != nil {
		panic(err)
	}

	return NewNumNode(val * float64(sign))
}

func (p *Parser) skip() {
	for p.index < len(p.input) {
		sym := p.input[p.index]
		if (sym != ' ') && (sym != '\t') && (sym != '\n') {
			break
		}
		p.index++
	}
}

func (p *Parser) getFieldName() string {
	p.index++

	name := ""
	for p.index < len(p.input) && p.get() != '"' {
		name = name + string(p.get())
		p.index++
	}
	p.index++
	return name
}

func isBool(v byte) bool {
	lowerV := strings.ToLower(string(v))
	return (lowerV == "t") || (lowerV == "f")
}

func isNumber(v byte) bool {
	return (v == '-') || ((v >= '0') && (v <= '9'))
}
