cmake_minimum_required(VERSION 3.24.2)
project(chess-ai) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-stdlib=libc++")

set(CMAKE_BUILD_TYPE Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_DEBUG "-Wall")

find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

add_executable(chess 
    src/main.cpp
    src/board/board.cpp
    src/gui/gui_main.cpp
    src/gui/piece_sprite.cpp
    src/board/piece.cpp
    src/engine/engine.cpp
    src/engine/engine_state.cpp
    src/board/move_result.cpp
    src/board/coordinate.cpp
)

# i have no idea how CMake finds these
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio)