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

}  // namespace

JsonValue JsonParser::parse(std::istream& input) const {
  skip(input);

  JsonValue value;

  if (input.peek() == '{') {
    value = parseObject(input);
  } else if (input.peek() == '[') {
    value = parseArray(input);
  } else if (input.peek() == '"') {
    value = parseString(input);
  } else {
    value = parseValue(input);
  }
  swallowComma(input);

  return value;
}

JsonValue JsonParser::parseObject(std::istream& input) const {
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

JsonValue JsonParser::parseArray(std::istream& input) const {
  input.get();

  JsonValue::Array array;

  while (input && input.peek() != ']') {
    const auto next = parse(input);
    array.push_back(next);
  }

  ensure_get(input, {']'});

  return JsonValue(array);
}

JsonValue JsonParser::parseString(std::istream& input) const {
  input.get();
  std::string value;

  while (input && input.peek() != '"') {
    value += input.get();
  }

  ensure_get(input, {'"'});

  return JsonValue(value);
}

JsonValue JsonParser::parseValue(std::istream& input) const {
  skip(input);

  const auto raw = getNumberOrKeyword(input);

  // Number
  try {
    const bool isFloatingPoint =
        (raw.find_first_of(".eE") != std::string::npos);
    if (isFloatingPoint) {
      const double value = std::stod(raw);
      return JsonValue(value);
    } else {
      const int value = std::stoi(raw);
      return JsonValue(value);
    }
  } catch (...) {
  }

  // Bool
  if (raw == std::string("true")) {
    return JsonValue(true);
  }
  if (raw == std::string("false")) {
    return JsonValue(false);
  }

  // Null
  if (raw == std::string("null")) {
    return JsonValue();
  }

  throw std::runtime_error(std::format("value {} is not supported", raw));
}

std::string JsonParser::parseKey(std::istream& input) const {
  const auto value = parseString(input);
  if (!value.isString()) {
    throw std::runtime_error("key is not a string");
  }
  return value.get<std::string>();
}

}  // namespace jsonparser
