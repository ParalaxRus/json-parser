import pytest

from json_parser.parser import JsonParser

test_suite = [
    ('{}', '{}'),
    ('[]', '[]'),
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
      '{"name":"Bob","age":30,"skills":["Python","C++","Docker"],"address":{"city":"New York","zip":"10001"},"employed":True,"email":null}'
      ),
]

@pytest.mark.parametrize("input, expected", test_suite)
def test_parser_v0(input, expected):
    parser = JsonParser()
    node = parser.parse(input)
    actual = str(node)
    assert actual == expected