cmake_minimum_required(VERSION 3.14)
project(QLOX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(Word_run ${SOURCE_FILES})

include_directories(Word_lib)
add_subdirectory(Word_lib)

target_link_libraries(Word_run Word_lib)

add_subdirectory(Google_tests)