cmake_minimum_required(VERSION 3.22)
project(theChance VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(theChance
  src/main.cpp
)

find_package(SDL2 QUIET CONFIG)
if (SDL2_FOUND)
  message(STATUS "Using SDL2 via CONFIG")
  target_link_libraries(theChance PRIVATE SDL2::SDL2)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED sdl2)
  target_include_directories(theChance PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_directories(theChance PRIVATE ${SDL2_LIBRARY_DIRS})
  target_link_libraries(theChance PRIVATE ${SDL2_LIBRARIES})
  add_definitions(${SDL2_CFLAGS_OTHER})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(theChance PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(theChance PRIVATE -fsanitize=address,undefined)
endif()
