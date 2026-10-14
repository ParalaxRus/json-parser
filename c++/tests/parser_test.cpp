#include "parser.h"

#include <gtest/gtest.h>

#include <string>

#include "value.h"

using jsonparser::JsonParser;
using jsonparser::JsonValue;

namespace {

void check(const std::string& input, const std::string& expected) {
  std::istringstream reader(input);

  JsonParser parser;
  const auto jsonValue = parser.parse(reader);

  const auto actual = jsonValue.serialize(true);
  EXPECT_EQ(actual, expected);

  std::istringstream reader1(actual);
  const auto deserializedJsonValue = parser.parse(reader1);
  EXPECT_EQ(jsonValue, deserializedJsonValue);
}

}  // namespace

TEST(ParserTest, EmptyJson) {
  const std::string input = "{}";
  check("{}", "{}");
}

TEST(ParserTest, SimpleJson) {
  const std::string input = R"(
  {
      "name": "Bob",
      "age": 30,
      "pos": -30.01,
      "weight": 185.5,
      "email": null
  }
  )";
  const std::string expected =
      R"({"age":30,"email":null,"name":"Bob","pos":-30.01,"weight":185.5})";
  check(input, expected);
}

TEST(ParserTest, ArrayJson) {
  const std::string input = R"(
  {
      "name": "Bob",
      "age": 30,
      "pos": -30.01,
      "email": null,
      "friends": ["andrey", "michael", "kattie"],
  }
  )";
  const std::string expected =
      R"({"age":30,"email":null,"friends":["andrey","michael","kattie"],"name":"Bob","pos":-30.01})";
  check(input, expected);
}

TEST(ParserTest, ObjectJson) {
  const std::string input = R"(
  {
      "name": "Bob",
      "age": 30,
      "email": null,
      "person": {
        "height": 6.0,
        "age": 16,
        "gpa": 3.1,
      }
  }
  )";
  const std::string expected =
      R"({"age":30,"email":null,"name":"Bob","person":{"age":16,"gpa":3.1,"height":6.0}})";
  check(input, expected);
}

TEST(ParserTest, MixedJson) {
  const std::string input = R"(
  {
      "name": "Bob",
      "age": 30,
      "skills": ["Python", "C++", "Docker",],
      "address": {
          "city": "New York",
          "zip": "10001",
      },
      "employed": true,
      "email": "bob@yahoo.com"
  }
  )";
  const std::string expected =
      R"({"address":{"city":"New York","zip":"10001"},"age":30,"email":"bob@yahoo.com","employed":true,"name":"Bob","skills":["Python","C++","Docker"]})";
  check(input, expected);
}
