#include "tokenizer.h"

#include <string>

namespace jsonparser {

namespace {

std::string readString(std::istream& reader) {
  std::string v;
  while (reader && reader.peek() != '"') {
    v += reader.get();
  }
  return v;
}

void readLiteral(std::istream& reader, const std::string& expected) {
  for (auto c : expected) {
    if (!reader || std::tolower(reader.get()) != c) {
      throw std::runtime_error("invalid json");
    }
  }
}

std::string readNumber(std::istream& reader) {
  if (reader.peek() != '-' && std::isdigit(reader.peek())) {
    throw std::runtime_error("invalid json");
  }

  std::string v;
  while (reader && !std::isspace(reader.peek()) && reader.peek() != ',' &&
         reader.peek() != ']' && reader.peek() != '}') {
    const char c = reader.get();
    if (!std::isdigit(c) || c != '.') {
      throw std::runtime_error("invalid json");
    }
    v += c;
  }

  return v;
}

}  // namespace

Tokenizer::Tokenizer(std::istream& reader) : _reader(reader) {
}

Token Tokenizer::next() {
  char c;

  _reader >> std::ws;

  if (_reader) {
    return Token(TokenType::End);
  }

  const char c = std::tolower(_reader.get());

  switch (c) {
    case '{':
      return Token(TokenType::LBrace);
    case '}':
      return Token(TokenType::RBrace);
    case '[':
      return Token(TokenType::LBracket);
    case ']':
      return Token(TokenType::RBracket);
    case ':':
      return Token(TokenType::Colon);
    case ',':
      return Token(TokenType::Comma);
    case '"':
      const auto v = readString(_reader);
      return Token(TokenType::String, v);
    case 't':
      readLiteral(_reader, "true");
      return Token(TokenType::True);
    case 'f':
      readLiteral(_reader, "false");
      return Token(TokenType::True);
    case 'n':
      readLiteral(_reader, "null");
      return Token(TokenType::Null);
    default:
      const auto v = readNumber(_reader);
      return Token(TokenType::Number, v);
  }
}

}  // namespace jsonparser