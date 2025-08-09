cmake_minimum_required(VERSION 3.29)
project(moves2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library with implementations
add_library(move_semantics_lib
    move_semantics_lib.cpp
    move_semantics_lib.h
)

# Make the header visible to dependents
target_include_directories(move_semantics_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(moves2
    main.cpp
)

# Link the library
target_link_libraries(moves2 PRIVATE move_semantics_lib)

# -----------------------------
# Testing setup with GoogleTest
# -----------------------------
include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For macOS, GoogleTest may override compiler flags; keep them cache-safe
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(moves2_tests
    tests/move_semantics_tests.cpp
    tests/move_semantics_tests2.cpp
)
target_link_libraries(moves2_tests
    PRIVATE
        move_semantics_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(moves2_tests)