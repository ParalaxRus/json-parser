cmake_minimum_required(VERSION 3.20)

project(MyCppProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(jsonparser
    src/main.cpp
    src/parser.cpp
    src/value.cpp
)

# GoogleTest setup

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(parser_tests
    tests/parser_test.cpp

    src/parser.cpp
    src/value.cpp
)

target_include_directories(parser_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(parser_tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(parser_tests)
