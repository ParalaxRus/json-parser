#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include "parser.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: jsonparser <json file>" << std::endl;
    return 1;
  }

  const auto path = fs::path(argv[1]);
  if (!fs::exists(path)) {
    std::cerr << std::format("{} not found", path.string()) << std::endl;
    return 1;
  }

  std::ifstream reader(path);
  jsonparser::JsonParser parser;
  const auto value = parser.parse(reader);
  std::cout << "json from file:\n" << value.serialize() << std::endl;

  return 0;
}
