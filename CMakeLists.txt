project(chess-ai) 
cmake_minimum_required(VERSION 3.24.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-stdlib=libc++")

find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

add_executable(chess 
    src/main.cpp
    src/board/board.cpp
    src/gui/gui_main.cpp
    src/gui/piece_sprite.cpp
    src/common/app_state.cpp
    src/board/engine.cpp
)

# i have no idea how CMake finds these
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio)