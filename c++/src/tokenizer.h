#pragma once

#include <istream>
#include <optional>
#include <vector>

namespace jsonparser {

enum class TokenType {
  LBrace,
  RBrace,
  LBracket,
  RBracket,
  Colon,
  Comma,
  String,
  Number,
  True,
  False,
  Null,
  End,
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
  Token(TokenType type, std::optional<std::string> value = std::nullopt)
      : type(type), value(value) {
  }
};

class Tokenizer {
 public:
  explicit Tokenizer(std::istream& reader);
  Token next();
  TokenType peekType();

 private:
  std::istream& _reader;
};

}  // namespace jsonparser