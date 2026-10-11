#pragma once

#include <istream>

#include "value.h"

namespace jsonparser {

class JsonParser {
 public:
  JsonValue parse(std::istream& input) const;

 private:
  JsonValue parseObject(std::istream& input) const;
  JsonValue parseArray(std::istream& input) const;
  JsonValue parseString(std::istream& input) const;
  JsonValue parseValue(std::istream& input) const;

  std::string parseKey(std::istream& input) const;
};

}  // namespace jsonparser
