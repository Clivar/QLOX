cmake_minimum_required(VERSION 3.21)
project(QLOX)

set(CMAKE_CXX_STANDARD 20)

add_executable(QLOX tests.cpp word.cpp word.h)
