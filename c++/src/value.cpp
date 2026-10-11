#include "value.h"

#include <cmath>
#include <format>
#include <map>
#include <sstream>
#include <stdexcept>

namespace jsonparser {

namespace {

template <typename Iterable>
void serializeObject(const Iterable& map,
                     std::string& serialized,
                     bool sorted) {
  for (const auto& [key, value] : map) {
    const auto serialized_value = value.serialize(sorted);
    serialized += std::format("\"{}\":{},", key, serialized_value);
  }

  if (!serialized.empty() && serialized.back() == ',') {
    serialized.pop_back();
  }
}

}  // namespace

std::string JsonValue::serialize(bool sorted) const {
  return JsonValue::serialize(*this, sorted);
}

std::string JsonValue::serialize(const JsonValue& value, bool sorted) {
  std::string serialized;

  if (value.isObject()) {
    const auto& object = value.get<JsonValue::Object>();
    serialized += '{';
    if (sorted) {
      std::map<std::string, JsonValue> sortedObject(object.begin(),
                                                    object.end());
      serializeObject(sortedObject, serialized, sorted);
    } else {
      serializeObject(object, serialized, sorted);
    }
    serialized += '}';
  } else if (value.isArray()) {
    serialized += '[';
    const auto& array = value.get<JsonValue::Array>();
    for (const auto& item : array) {
      const auto serialized_item = JsonValue::serialize(item, false);
      serialized += std::format("{},", serialized_item);
    }
    if (serialized.back() == ',') {
      serialized.pop_back();
    }
    serialized += ']';
  } else if (value.isString()) {
    serialized += '"';
    serialized += value.get<std::string>();
    serialized += '"';
  } else if (value.isDouble()) {
    std::ostringstream oss;
    const double val = value.get<double>();
    oss << val;
    if (std::isfinite(val) && std::trunc(val) == val) {
      oss << ".0";
    }
    serialized += oss.str();
  } else if (value.isInt()) {
    serialized += std::to_string(value.get<int>());
  } else if (value.isBool()) {
    if (value.get<bool>()) {
      serialized += "true";
    } else {
      serialized += "false";
    }
  } else if (value.isNull()) {
    serialized += "null";
  } else {
    throw std::runtime_error("json value type is not supported");
  }

  return serialized;
}

bool JsonValue::operator==(const JsonValue& other) const {
  return JsonValue::equal(*this, other);
}

bool JsonValue::equal(const JsonValue& first, const JsonValue& second) {
  return (first._value == second._value);
}

}  // namespace jsonparser
