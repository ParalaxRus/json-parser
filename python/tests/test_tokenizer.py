import pytest

from json_parser.tokenizer import JsonTokenizer
from json_parser.token import JsonTokenType, JsonToken

test_suite = [
    ("", []),
    ("{}", [JsonToken(JsonTokenType.LEFT_BRACE), JsonToken(JsonTokenType.RIGHT_BRACE)]),
    ("[]", [JsonToken(JsonTokenType.LEFT_BRACKET), JsonToken(JsonTokenType.RIGHT_BRACKET)]),
    ("""
      {
        "name": "Bob",
        "age": 30,
        "skills": ["Python", "C++", "Docker"],
        "address": {
          "city": "New York",
          "zip": "10001"
        },
        "employed": True,
        "email": null
      }
      """,
      [JsonToken(JsonTokenType.LEFT_BRACE),
       JsonToken(JsonTokenType.STRING, "name"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.STRING, "Bob"),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "age"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.NUMBER, "30"),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "skills"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.LEFT_BRACKET),
       JsonToken(JsonTokenType.STRING, "Python"),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "C++"),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "Docker"),
       JsonToken(JsonTokenType.RIGHT_BRACKET),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "address"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.LEFT_BRACE),
       JsonToken(JsonTokenType.STRING, "city"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.STRING, "New York"),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "zip"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.STRING, "10001"),
       JsonToken(JsonTokenType.RIGHT_BRACE),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "employed"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.TRUE),
       JsonToken(JsonTokenType.COMMA),
       JsonToken(JsonTokenType.STRING, "email"),
       JsonToken(JsonTokenType.COLON),
       JsonToken(JsonTokenType.NULL),
       JsonToken(JsonTokenType.RIGHT_BRACE)
       ]),
]

@pytest.mark.parametrize("input, expected", test_suite)
def test_tokenizer_v0(input, expected):
    tokenizer = JsonTokenizer(input)
    tokens = tokenizer.tokenize()
    assert tokens == expected
