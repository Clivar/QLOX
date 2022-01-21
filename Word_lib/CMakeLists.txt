project(Word)

set(HEADER_FILES
        word.h
        matrix.cpp matrix.h)

set(SOURCE_FILES
        word.cpp matrix.cpp matrix.h)

add_library(Word_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})