from json_parser.token import JsonToken, JsonTokenType


class JsonTokenizer:

    _SKIPPABLE = {' ', '\t', '\n'}

    def __init__(self, val: str):
        self._val = val
        self._pos = 0

    @staticmethod
    def _is_number(c: str) -> bool:
        if len(c) == 0:
            return False
        return (c == '-') or c.isdigit()
    
    @staticmethod
    def _is_alnum(c: str) -> bool:
        if len(c) == 0:
            return False
        return c.isalnum()
    
    @classmethod
    def _skippable(cls, c: str) -> bool:
        return c in cls._SKIPPABLE

    def _skip(self) -> None:
        while self._pos < len(self._val) and JsonTokenizer._skippable(self._val[self._pos]):
            self._pos = self._pos + 1

    def _peek(self) -> str:
        if (self._pos >= len(self._val)):
            return ""
        return self._val[self._pos]

    def _get(self) -> str:
        if (self._pos >= len(self._val)):
            return ""
        v = self._val[self._pos]
        self._pos += 1
        return v
    
    def _parse_str(self) -> JsonToken:
        val = ''
        while True: 
            c = self._get()
            if c == '"':
                break
            val += c
        return JsonToken(JsonTokenType.STRING, val)
    
    def _parse_num(self) -> JsonToken:
        val = ''
        if self._peek() == '-':
            val = '-'
            self._get()
        while True: 
            c = self._peek()
            if not c.isdigit():
                break
            c = self._get()
            val += c
        return JsonToken(JsonTokenType.NUMBER, val)
    
    def _parse_keyword(self) -> JsonToken:
        val = ''
        while True:
            c = self._peek()
            if not JsonTokenizer._is_alnum(c):
                break
            c = self._get()
            val += c
        val = val.lower()
        if val == 'true':
            return JsonToken(JsonTokenType.TRUE)
        if val == 'false':
            return JsonToken(JsonTokenType.FALSE)
        if val == 'null':
            return JsonToken(JsonTokenType.NULL)
        return JsonToken(JsonTokenType.UNKNOWN)
    

    def _get_next_token(self) -> JsonTokenType:
        self._skip()
        c = self._peek()

        match c:
            case '{':
                self._get()
                return JsonToken(JsonTokenType.LEFT_BRACE)
            case '}':
                self._get()
                return JsonToken(JsonTokenType.RIGHT_BRACE)
            case '[':
                self._get()
                return JsonToken(JsonTokenType.LEFT_BRACKET)
            case ']':
                self._get()
                return JsonToken(JsonTokenType.RIGHT_BRACKET)
            case ':':
                self._get()
                return JsonToken(JsonTokenType.COLON)
            case ',':
                self._get()
                return JsonToken(JsonTokenType.COMMA)
            case '"':
                self._get()
                return self._parse_str()
            case "":
                return JsonToken(JsonTokenType.JSON_END)
            case _:
                if JsonTokenizer._is_number(c):
                    return self._parse_num()
                if JsonTokenizer._is_alnum(c):
                    return self._parse_keyword()
                raise RuntimeError(f"Unknown character {c}")
            

    def tokenize(self) -> list['JsonToken']:
        tokens = []
        while True:
            token = self._get_next_token()
            if token.type == JsonTokenType.JSON_END:
                break
            tokens.append(token)
        return tokens
