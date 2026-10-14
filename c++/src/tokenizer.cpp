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

Tokenizer::Tokenizer(std::istream& reader)
    : _reader(reader), _token(Token(TokenType::End)) {
}

bool Tokenizer::next() {
  if (!_reader) {
    _token = Token(TokenType::End);
    return false;
  }

  _reader >> std::ws;

  if (!_reader) {
    _token = Token(TokenType::End);
    return false;
  }

  const char c = std::tolower(_reader.get());

  switch (c) {
    case '{':
      _token = Token(TokenType::LBrace);
      break;
    case '}':
      _token = Token(TokenType::RBrace);
      break;
    case '[':
      _token = Token(TokenType::LBracket);
      break;
    case ']':
      _token = Token(TokenType::RBracket);
      break;
    case ':':
      _token = Token(TokenType::Colon);
      break;
    case ',':
      _token = Token(TokenType::Comma);
      break;
    case '"': {
      const auto str = readString(_reader);
      _token = Token(TokenType::String, str);
      break;
    }
    case 't':
      readLiteral(_reader, "true");
      _token = Token(TokenType::True);
      break;
    case 'f':
      readLiteral(_reader, "false");
      _token = Token(TokenType::False);
      break;
    case 'n':
      readLiteral(_reader, "null");
      _token = Token(TokenType::Null);
      break;
    default: {
      const auto num = readNumber(_reader);
      _token = Token(TokenType::Number, num);
    }
  }

  return true;
}

}  // namespace jsonparser
