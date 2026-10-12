#include "parser.h"

#include <algorithm>
#include <format>
#include <stdexcept>
#include <unordered_set>

namespace jsonparser {

namespace {

void swallowComma(std::istream& input) {
  if (input && input.peek() == ',') {
    input.get();
  }
}

void skip(std::istream& input) {
  while (input && std::isspace(input.peek())) {
    input.get();
  }
}

char ensure_get(std::istream& input) {
  if (!input) {
    throw std::runtime_error("invalid json");
  }
  return input.get();
}

void ensure_get(std::istream& input, const std::unordered_set<char>& expected) {
  skip(input);
  auto actual = ensure_get(input);
  if (expected.count(actual) == 0) {
    const std::string info(expected.begin(), expected.end());
    throw std::runtime_error(std::format("expected {} got {}", info, actual));
  }
}

std::string getNumberOrKeyword(std::istream& input) {
  std::string value;

  while (input && !std::isspace(input.peek()) && input.peek() != ',' &&
         input.peek() != ']' && input.peek() != '}') {
    value += input.get();
  }

  std::transform(
      value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return std::tolower(c);
      });

  return value;
}

JsonValue parseNumber(const std::string& v) {
  // Number
  try {
    const bool isFloatingPoint = (v.find_first_of(".eE") != std::string::npos);
    if (isFloatingPoint) {
      const double value = std::stod(v);
      return JsonValue(value);
    } else {
      const int value = std::stoi(v);
      return JsonValue(value);
    }
  } catch (const std::exception& exc) {
    throw std::runtime_error(std::format("invalid json, error: {}"),
                             exc.what());
  }
}

}  // namespace

JsonValue JsonParser::parse(std::istream& input) const {
  Tokenizer tokenizer(input);
  const auto token = tokenizer.next();
  switch (token.type) {
    case TokenType::LBrace:
      return parseObject(tokenizer);
    case TokenType::LBracket:
      return parseArray(tokenizer);
    case TokenType::String:
      return JsonValue(*token.value);
    case TokenType::Number:
      return parseNumber(*token.value);
    case TokenType::False:
      return JsonValue(false);
    case TokenType::True:
      return JsonValue(true);
    case TokenType::Null:
      return JsonValue();
    default:
      // skip
  }
}

JsonValue JsonParser::parseObject(Tokenizer& tokenizer) const {
  input.get();

  JsonValue::Object object;

  while (input && input.peek() != '}') {
    skip(input);

    if (input.peek() != '"') {
      throw std::runtime_error("key not found");
    }
    const auto key = parseKey(input);
    ensure_get(input, {':'});
    const auto value = parse(input);
    if (object.count(key) > 0) {
      throw std::runtime_error(std::format("duplicate key {} found", key));
    }
    object[key] = value;

    skip(input);
  }

  ensure_get(input, {'}'});

  return JsonValue(object);
}

JsonValue JsonParser::parseArray(Tokenizer& tokenizer) const {
  JsonValue::Array array;

  while (true) {
    const auto token = tokenizer.next();
    if (token.type == TokenType::LBracket) {
      break;
    }
    if (token.type == TokenType::End) {
      throw std::runtime_error("invalid json");
    }
    if (token.type == TokenType::Comma) {
      continue;
    }

    const auto next = parse(tokenizer);
    array.push_back(next);
  }

  return JsonValue(array);
}

}  // namespace jsonparser
