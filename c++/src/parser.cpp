#include "parser.h"

#include <algorithm>
#include <format>
#include <stdexcept>
#include <unordered_set>

namespace jsonparser {

namespace {

void ensureValue(const std::optional<JsonValue>& value) {
  if (!value) {
    throw std::runtime_error("invalid json");
  }
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
    throw std::runtime_error(
        std::format("invalid json, error: {}", exc.what()));
  }
}

}  // namespace

JsonValue JsonParser::parse(std::istream& input) const {
  Tokenizer tokenizer(input);
  const auto value = parse(tokenizer);
  ensureValue(value);
  return *value;
}

std::optional<JsonValue> JsonParser::parse(Tokenizer& tokenizer) const {
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
      return std::nullopt;
  }
}

JsonValue JsonParser::parseObject(Tokenizer& tokenizer) const {
  JsonValue::Object object;

  while (true) {
    const auto key = tokenizer.next();
    if (key.type == TokenType::RBrace) {
      break;
    }
    if (key.type == TokenType::Comma) {
      continue;
    }
    if (key.type != TokenType::String) {
      throw std::runtime_error("invalid json");
    }
    if (object.count(*key.value) > 0) {
      throw std::runtime_error(
          std::format("duplicate key {} found", *key.value));
    }
    const auto colon = tokenizer.next();
    if (colon.type != TokenType::Colon) {
      throw std::runtime_error("invalid json");
    }
    const auto value = parse(tokenizer);
    ensureValue(value);
    object[*key.value] = *value;
  }

  return JsonValue(object);
}

JsonValue JsonParser::parseArray(Tokenizer& tokenizer) const {
  JsonValue::Array array;

  while (true) {
    const auto tokenType = tokenizer.peekType();
    if (tokenType == TokenType::End) {
      throw std::runtime_error("invalid json");
    }
    if (tokenType == TokenType::Comma) {
      tokenizer.next();
      continue;
    }
    if (tokenType == TokenType::RBracket) {
      tokenizer.next();
      break;
    }
    const auto value = parse(tokenizer);
    ensureValue(value);
    array.push_back(*value);
  }

  return JsonValue(array);
}

}  // namespace jsonparser
