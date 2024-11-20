cmake_minimum_required(VERSION 3.29)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(chess src/main.cpp
        src/Piece.cpp
        src/Piece.h
        src/themes/Themes.h
        src/themes/ClassicThemeManager.cpp
        src/Game.cpp
        src/Game.h
        src/Renderer.cpp
        src/Renderer.h
        src/Board.cpp
        src/Board.h
        src/AI.cpp
        src/AI.h
        src/UI.cpp
        src/UI.h
        src/moves.cpp
        src/moves.h
        src/move_generation.cpp
        src/move_generation.h
)

target_include_directories(chess PRIVATE src/themes src)

target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
