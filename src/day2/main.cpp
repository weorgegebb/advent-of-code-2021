#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace {

auto CheckInputIsValid(const int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " path-to-input" << std::endl;
    return false;
  }
  return true;
}

enum class Direction { FORWARD, DOWN, UP, NONE };

struct Command {
  Command(const Direction dir, const int dist)
      : direction(dir), distance(dist) {}
  const Direction direction = Direction::NONE;
  const int distance = 0;
};

auto GetInputData(const char* path) {
  std::ifstream input_file(path);
  std::string line;
  char command;
  int distance;
  std::vector<Command> commands;

  while (std::getline(input_file, line)) {
    if (line.size() > 0) {
      auto space_itr = std::find(line.begin(), line.end(), ' ');
      command = *line.begin();
      distance = std::stoi(std::string(space_itr, line.end()));

      switch (command) {
        case 'f':
          commands.emplace_back(Direction::FORWARD, distance);
          break;
        case 'd':
          commands.emplace_back(Direction::DOWN, distance);
          break;
        case 'u':
          commands.emplace_back(Direction::UP, distance);
          break;
        default:
          commands.emplace_back(Direction::NONE, distance);
          break;
      }
    }
  }
  return commands;
}

struct SubmarineState {
  int horizontal;
  int depth;
  int aim;
};

auto CommandSubmarine(const Command& command, SubmarineState& state) {
  switch (command.direction) {
    case Direction::FORWARD:
      state.horizontal += command.distance;
      break;
    case Direction::DOWN:
      state.depth += command.distance;
      break;
    case Direction::UP:
      state.depth -= command.distance;
      break;
    case Direction::NONE:
      break;
    default:
      break;
  }
}

auto CommandSubmarineWithAim(const Command& command, SubmarineState& state) {
  switch (command.direction) {
    case Direction::FORWARD:
      state.horizontal += command.distance;
      state.depth += (state.aim * command.distance);
      break;
    case Direction::DOWN:
      state.aim += command.distance;
      break;
    case Direction::UP:
      state.aim -= command.distance;
      break;
    case Direction::NONE:
      break;
    default:
      break;
  }
}

auto ApplyCommandsToSubmarine(const std::vector<Command>& commands,
                              SubmarineState& state) {
  for (const auto& command : commands) {
    CommandSubmarine(command, state);
  }
}

auto ApplyAimCommandsToSubmarine(const std::vector<Command>& commands,
                                 SubmarineState& state) {
  for (const auto& command : commands) {
    CommandSubmarineWithAim(command, state);
  }
}

auto ApplyCommandsToStateWithPolicy(
    const std::vector<Command>& commands,
    const std::function<void(const std::vector<Command>&, SubmarineState&)>&
        command_fcn) {
  auto submarine_state = SubmarineState();
  command_fcn(commands, submarine_state);
  return submarine_state;
}

}  // namespace

auto main(int argc, char* argv[]) -> int {
  if (!CheckInputIsValid(argc, argv)) {
    return 0;
  }

  const auto& file_path = argv[1];
  const auto commands = GetInputData(file_path);

  // Part 1
  auto submarine_state =
      ApplyCommandsToStateWithPolicy(commands, ApplyCommandsToSubmarine);
  std::cout << "Part 1: depth x horizontal = "
            << (submarine_state.depth * submarine_state.horizontal)
            << std::endl;

  // Part 2
  auto submarine_state_with_aim =
      ApplyCommandsToStateWithPolicy(commands, ApplyAimCommandsToSubmarine);
  std::cout << "Part 2: depth x horizontal = "
            << (submarine_state_with_aim.depth *
                submarine_state_with_aim.horizontal)
            << std::endl;

  return 0;
}
