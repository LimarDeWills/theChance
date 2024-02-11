cmake_minimum_required(VERSION 3.10)

project(Logger VERSION 1.0 DESCRIPTION "A simple logging module")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(SOURCES
    src/strategy/compositeLogStrategy.cpp
    src/strategy/consoleLogStrategy.cpp
    src/strategy/fileLogStrategy.cpp
    src/logger.cpp
)

add_library(Logger STATIC ${SOURCES})

target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
