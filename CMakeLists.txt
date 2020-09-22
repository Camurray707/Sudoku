cmake_minimum_required(VERSION 3.16)
project(project1 C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(project1
        sudoku-project-master/sudoku.c)
find_package (Threads REQUIRED)

target_link_libraries(project1
        ${CMAKE_THREAD_LIBS_INIT}
        )