#include "tokenizer.h"

#include <string>

namespace jsonparser {

namespace {

std::string readString(std::istream& reader) {
  std::string v;
  while (reader && reader.peek() != '"') {
    v += reader.get();
  }
  reader.get();
  return v;
}

void readLiteral(std::istream& reader, const std::string& expected) {
  reader.unget();
  for (auto c : expected) {
    if (!reader || std::tolower(reader.get()) != c) {
      throw std::runtime_error("invalid json");
    }
  }
}

std::string readNumber(std::istream& reader) {
  reader.unget();
  if (reader.peek() != '-' && !std::isdigit(reader.peek())) {
    throw std::runtime_error("invalid json");
  }

  std::string v;
  v += reader.get();
  while (reader && !std::isspace(reader.peek()) && reader.peek() != ',' &&
         reader.peek() != ']' && reader.peek() != '}') {
    const char c = reader.get();
    if (!std::isdigit(c) && c != '.') {
      throw std::runtime_error("invalid json");
    }
    v += c;
  }

  return v;
}

}  // namespace

Tokenizer::Tokenizer(std::istream& reader) : _reader(reader) {
}

TokenType Tokenizer::peekType() {
  if (!_reader) {
    return TokenType::End;
  }

  _reader >> std::ws;

  if (!_reader) {
    return TokenType::End;
  }

  const char c = std::tolower(_reader.peek());

  switch (c) {
    case '{':
      return TokenType::LBrace;
    case '}':
      return TokenType::RBrace;
    case '[':
      return TokenType::LBracket;
    case ']':
      return TokenType::RBracket;
    case ':':
      return TokenType::Colon;
    case ',':
      return TokenType::Comma;
    case '"': {
      return TokenType::String;
    }
    case 't':
      return TokenType::True;
    case 'f':
      return TokenType::False;
    case 'n':
      return TokenType::Null;
    default: {
      return TokenType::Number;
    }
  }

  throw std::runtime_error("should not happen");
}

Token Tokenizer::next() {
  if (!_reader) {
    return Token(TokenType::End);
  }

  _reader >> std::ws;

  if (!_reader) {
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
    case '"': {
      const auto str = readString(_reader);
      return Token(TokenType::String, str);
    }
    case 't':
      readLiteral(_reader, "true");
      return Token(TokenType::True);
    case 'f':
      readLiteral(_reader, "false");
      return Token(TokenType::False);
    case 'n':
      readLiteral(_reader, "null");
      return Token(TokenType::Null);
    default: {
      const auto num = readNumber(_reader);
      return Token(TokenType::Number, num);
    }
  }

  throw std::runtime_error("should not happen");
}

}  // namespace jsonparser