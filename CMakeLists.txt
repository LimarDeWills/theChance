cmake_minimum_required(VERSION 3.22)
project(theChance VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(theChance
  src/main.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(theChance PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(theChance PRIVATE -fsanitize=address,undefined)
endif()
