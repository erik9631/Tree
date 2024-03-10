cmake_minimum_required(VERSION 3.27)

PROJECT(catch_amalgamated)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC catch_amalgamated.hpp catch_amalgamated.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})