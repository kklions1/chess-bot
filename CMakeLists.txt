cmake_minimum_required(VERSION 3.24.2)

project(chess-ai) 

set(CMAKE_CXX_FLAGS "-std=c++20")

add_executable(chess src/main.cpp src/board/board.cpp)
