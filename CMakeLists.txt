cmake_minimum_required(VERSION 3.29)
project(chess)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

set(CORE_FILES src/core/Piece.cpp
        src/core/Game.cpp
        src/core/Board.cpp
        src/core/AI.cpp
        src/core/moves.cpp
        src/core/move_generation.cpp
)

set(SOURCE_FILES src/main.cpp
        src/themes/ClassicThemeManager.cpp
        src/UI.cpp
        src/Renderer.cpp
        src/InputManager.cpp
        src/InputManager.h
)

add_executable(chess ${SOURCE_FILES} ${CORE_FILES})

target_include_directories(chess PRIVATE src/themes src src/core)

target_link_libraries(chess sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

enable_testing()

add_executable(
        GTests_run
        Google_tests/BoardTests.cpp
        ${CORE_FILES}
        Google_tests/MoveGenerationTest.cpp
)

target_include_directories(GTests_run PRIVATE src/core)

target_link_libraries(
        GTests_run
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(GTests_run)
