cmake_minimum_required(VERSION 3.21)

# Set the project name and version.
project(AdventOfCode2021 VERSION 0.1)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use Ninja.
set(CMAKE_GENERATOR "Ninja")

# Export compile_commands.json file.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../build/bin")

add_subdirectory(demo)
add_subdirectory(day1)
