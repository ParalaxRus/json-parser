#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace jsonparser {

class JsonValue {
 public:
  using Array = std::vector<JsonValue>;
  using Object = std::unordered_map<std::string, JsonValue>;

  JsonValue() : _value(nullptr) {
  }

  explicit JsonValue(bool val) : _value(val) {
  }

  explicit JsonValue(int val) : _value(val) {
  }

  explicit JsonValue(double val) : _value(val) {
  }

  explicit JsonValue(std::string val) : _value(std::move(val)) {
  }

  explicit JsonValue(Array val) : _value(std::move(val)) {
  }

  explicit JsonValue(Object val) : _value(std::move(val)) {
  }

  bool isNull() const {
    return std::holds_alternative<std::nullptr_t>(_value);
  }

  bool isBool() const {
    return std::holds_alternative<bool>(_value);
  }

  bool isInt() const {
    return std::holds_alternative<int>(_value);
  }

  bool isDouble() const {
    return std::holds_alternative<double>(_value);
  }

  bool isString() const {
    return std::holds_alternative<std::string>(_value);
  }

  bool isArray() const {
    return std::holds_alternative<JsonValue::Array>(_value);
  }

  bool isObject() const {
    return std::holds_alternative<JsonValue::Object>(_value);
  }

  template <typename T>
  T& get() {
    return std::get<T>(_value);
  }

  template <typename T>
  const T& get() const {
    return std::get<T>(_value);
  }

  bool operator==(const JsonValue& other) const;

  std::string serialize(bool sorted = false) const;

 private:
  using Value = std::
      variant<std::nullptr_t, bool, int, double, std::string, Array, Object>;
  Value _value;

  static std::string serialize(const JsonValue& value, bool sorted);

  static bool equal(const JsonValue& first, const JsonValue& second);
};

}  // namespace jsonparser
