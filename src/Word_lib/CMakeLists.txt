project(Word)

set(HEADER_FILES
        word.h matrix.h)

set(SOURCE_FILES
        word.cpp matrix.cpp)

add_library(Word_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})