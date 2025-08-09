cmake_minimum_required(VERSION 4.0)
project(moves2)

set(CMAKE_CXX_STANDARD 20)

add_executable(moves2 main.cpp)

# Add the move semantics library
add_library(move_semantics_lib
        move_semantics_lib.h
        move_semantics_lib.cpp
        move_semantics_lib.cpp
)

# Link the library to the main executable
target_link_libraries(moves2 move_semantics_lib)

# Ensure the library can find its header
target_include_directories(move_semantics_lib PUBLIC .)