cmake_minimum_required(VERSION 3.29)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(chess main.cpp
        piece.cpp
        piece.h)

target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
