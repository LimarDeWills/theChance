cmake_minimum_required(VERSION 3.10)
project(Sandbox)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../logger Logger)

include_directories(../logger/include)

add_executable(Sandbox src/main.cpp)

target_link_libraries(Sandbox Logger)