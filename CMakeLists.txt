cmake_minimum_required(VERSION 3.27)
PROJECT(Tree)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE "")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(libs/catch2)