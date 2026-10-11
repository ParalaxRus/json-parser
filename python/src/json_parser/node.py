from enum import Enum

class JsonNode:

    class NodeType(Enum):
        OBJECT = 1
        ARRAY = 2
        STR = 3
        NUM = 4
        BOOL = 5
        NULL = 6

    def __init__(self, type = NodeType.NULL, value = None):
        self._type = type
        self._value = value
        
    def __str__(self) -> str:
        match self._type:
            case JsonNode.NodeType.OBJECT:
                objAsStr = ''
                for key in self._value:
                    value = self._value[key]
                    objAsStr += f'"{key}":{str(value)},'
                if len(objAsStr) > 0:
                    objAsStr = objAsStr[:-1]
                return f"{{{objAsStr}}}"
            case JsonNode.NodeType.ARRAY:
                arrAsStr = ''
                for item in self._value:
                    arrAsStr += str(item) + ','
                if len(arrAsStr) > 0:
                    arrAsStr = arrAsStr[:-1]
                return f"[{arrAsStr}]"
            case JsonNode.NodeType.STR:
                return f'"{self._value}"'
            case JsonNode.NodeType.NUM:
                return f'{self._value}'
            case JsonNode.NodeType.BOOL:
                return str(self._value)
            case JsonNode.NodeType.NULL:
                return "null"
            case _:
                raise RuntimeError()
            
    def __repr__(self):
         return f"{self._type} {self._value}"

    @classmethod
    def create_object(cls, val: dict[str, 'JsonNode'] = {}) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.OBJECT, {})
    
    @classmethod
    def create_list(cls) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.ARRAY, [])
    
    @classmethod
    def create_string(cls, val: str) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.STR, val)
    
    @classmethod
    def create_number(cls, val: float) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.NUM, val)
    
    @classmethod
    def create_bool(cls, val: bool) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.BOOL, val)
    
    @classmethod
    def create_null(cls) -> 'JsonNode':
        return JsonNode(JsonNode.NodeType.NULL)

