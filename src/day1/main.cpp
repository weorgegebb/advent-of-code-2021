#include <deque>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace {

auto CheckInputIsValid(const int argc, char* argv[]) -> bool {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " path-to-input" << std::endl;
    return false;
  }
  return true;
}

auto GetInputData(const char* path) -> std::vector<int> {
  std::ifstream input_file(path);
  std::string line;
  std::vector<int> depth_data;

  while (std::getline(input_file, line)) {
    if (line.size() > 0) {
      depth_data.push_back(std::stoi(line));
    }
  }
  return depth_data;
}

auto NumberOfIncreasingDepths(const std::vector<int>& depths) -> int {
  auto number_of_increasing_depths = 0;
  for (auto i = 1; i < depths.size(); ++i) {
    if (depths[i] > depths[i - 1]) {
      ++number_of_increasing_depths;
    };
  }
  return number_of_increasing_depths;
}

class SlidingWindow {
 public:
  SlidingWindow(const int window_size) : window_size_(window_size) {}

 public:
  auto Push(const int value) -> int {
    queue.push_back(value);

    if (queue.size() > window_size_) {
      queue.pop_front();
    }

    auto sum = std::accumulate(
        queue.begin(), queue.end(), 0,
        [](const auto& sum, const auto& value) { return sum + value; });
    return sum;
  }

 private:
  const int window_size_;
  std::deque<int> queue;
};

auto NumberOfIncreasingDepthsWithSlidingWindow(const std::vector<int>& depths,
                                               const int window_size) -> int {
  auto sliding_window = SlidingWindow(window_size);

  // Load up window
  auto current_window_sum = 0;
  for (auto i = 0; i < window_size; ++i) {
    current_window_sum = sliding_window.Push(depths[i]);
  }

  auto number_of_increasing_depths = 0;
  auto previous_window_sum = 0;
  for (auto i = window_size; i < depths.size(); ++i) {
    previous_window_sum = current_window_sum;
    current_window_sum = sliding_window.Push(depths[i]);

    if (current_window_sum > previous_window_sum) {
      ++number_of_increasing_depths;
    }
  }

  return number_of_increasing_depths;
}

}  // namespace

auto main(int argc, char* argv[]) -> int {
  if (!CheckInputIsValid(argc, argv)) {
    return 0;
  }

  const auto& file_path = argv[1];
  const auto depths = GetInputData(file_path);

  // Part 1
  std::cout << "Part 1: The number of incresing depths is "
            << NumberOfIncreasingDepths(depths) << std::endl;

  // Part 2
  constexpr auto window_size = 3;

  const auto test_depths =
      std::vector{199, 200, 208, 210, 200, 207, 240, 269, 260, 263};
  std::cout
      << "Part 2 Test: The number of incresing depths in the sliding window is "
      << NumberOfIncreasingDepthsWithSlidingWindow(test_depths, window_size)
      << ", it should be 5" << std::endl;
  std::cout
      << "Part 2: The number of incresing depths in the sliding window is "
      << NumberOfIncreasingDepthsWithSlidingWindow(depths, window_size)
      << std::endl;

  return 0;
}
