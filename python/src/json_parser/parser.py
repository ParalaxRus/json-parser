from json_parser.node import JsonNode
from json_parser.token import JsonTokenType
from json_parser.tokenizer import JsonTokenizer

class JsonParser:

    def __init__(self):
        self._idx = 0
        self._tokens = []

    def _parse_object(self) -> JsonNode:
        node = JsonNode.create_object()
        self._idx += 1
        while self._tokens[self._idx].type != JsonTokenType.RIGHT_BRACE:
            next = self._tokens[self._idx]
            if next.type == JsonTokenType.COMMA:
                self._idx += 1
                continue
            if next.type != JsonTokenType.STRING:
                raise RuntimeError()
            key = next.value
            self._idx += 1
            next = self._tokens[self._idx]
            if next.type != JsonTokenType.COLON:
                raise RuntimeError()
            self._idx += 1
            node._value[key] = self._parse()
        return node


    def _parse_list(self) -> JsonNode:
        node = JsonNode.create_list()
        self._idx += 1
        while self._tokens[self._idx].type != JsonTokenType.RIGHT_BRACKET:
            if self._tokens[self._idx].type == JsonTokenType.COMMA:
                self._idx += 1
                continue
            next = self._parse()
            node._value.append(next)
        return node

    def _parse(self) -> JsonNode:
        if self._idx >= len(self._tokens):
            return JsonNode(JsonNode.NodeType.NULL)
        node = None
        match self._tokens[self._idx].type:
            case JsonTokenType.LEFT_BRACE:
                node = self._parse_object()
            case JsonTokenType.LEFT_BRACKET:
                node = self._parse_list()
            case JsonTokenType.STRING:
                node = JsonNode.create_string(self._tokens[self._idx].value)
            case JsonTokenType.NUMBER:
                node = JsonNode.create_number(self._tokens[self._idx].value)
            case JsonTokenType.TRUE:
                node = JsonNode.create_bool(True)
            case JsonTokenType.FALSE:
                node = JsonNode.create_bool(False)
            case JsonTokenType.NULL:
                node = JsonNode.create_null()
        self._idx += 1
        return node
        
    def parse(self, val: str) -> JsonNode:
        tokenizer = JsonTokenizer(val)
        self._idx = 0
        self._tokens = tokenizer.tokenize()
        return self._parse()

