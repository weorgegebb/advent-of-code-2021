#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
auto CheckInputIsValid(const int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " path-to-input" << std::endl;
    return false;
  }
  return true;
}

auto GetInputData(const char* path) {
  std::ifstream input_file(path);
  std::string line;
  std::vector<std::string> output_lines;

  while (std::getline(input_file, line)) {
    if (line.size() > 0) {
      output_lines.push_back(line);
    }
  }
  return output_lines;
}
}  // namespace

auto main(int argc, char* argv[]) -> int {
  if (!CheckInputIsValid(argc, argv)) {
    return 0;
  }

  const auto& file_path = argv[1];
  const auto report = GetInputData(file_path);

  // Part 1

  // Part 2

  return 0;
}
