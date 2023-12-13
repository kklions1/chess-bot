project(chess-ai) 
cmake_minimum_required(VERSION 3.24.2)
# add_definitions("-std=c++20")

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()

add_executable(chess 
    src/main.cpp
    src/board/board.cpp
    src/gui/gui_main.cpp
)
# target_link_libraries(chess ${CONAN_LIBS})
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio)