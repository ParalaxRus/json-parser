#pragma once

#include <istream>
#include <optional>

#include "tokenizer.h"
#include "value.h"

namespace jsonparser {

class JsonParser {
 public:
  JsonValue parse(std::istream& input) const;

 private:
  std::optional<JsonValue> parse(Tokenizer& tokenizer,
                                 bool advance = true) const;
  JsonValue parseObject(Tokenizer& tokenizer) const;
  JsonValue parseArray(Tokenizer& tokenizer) const;
  JsonValue parseString(Tokenizer& tokenizer) const;
  JsonValue parseValue(Tokenizer& tokenizer) const;
  std::string parseKey(Tokenizer& tokenizer) const;
};

}  // namespace jsonparser
