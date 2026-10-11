from enum import Enum

class JsonTokenType(Enum):
        LEFT_BRACE = 1 # {
        RIGHT_BRACE = 2 # }
        LEFT_BRACKET = 3 # [
        RIGHT_BRACKET = 4 # ]
        COLON = 5 # :
        COMMA = 6 # ,
        STRING = 7
        NUMBER = 8
        TRUE = 9
        FALSE = 10
        NULL = 11
        JSON_END = 12
        UNKNOWN = 13

class JsonToken:
    def __init__(self, type: JsonTokenType, value: str = ''):
        self.type = type
        self.value = value

    def __eq__(self, other):
        if not isinstance(other, JsonToken):
            return False
        return self.type == other.type and self.value == other.value
    
    def __repr__(self):
         return f"{self.type} {self.value}"