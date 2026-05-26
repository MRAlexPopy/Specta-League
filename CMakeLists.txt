cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "specta_league")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "include/*.h")
include_directories(include)

add_executable(${MAIN_EXECUTABLE_NAME} main.cpp ${SOURCES})

option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(USE_ASAN "Use Address Sanitizer" OFF)
option(USE_MSAN "Use Memory Sanitizer" OFF)
option(USE_UBSAN "Use Undefined Behavior Sanitizer" OFF)

target_compile_options(${MAIN_EXECUTABLE_NAME} PRIVATE
    -Wall -Wextra -pedantic
    $<$<BOOL:${WARNINGS_AS_ERRORS}>:-Werror>
)

if(USE_ASAN)
    target_compile_options(${MAIN_EXECUTABLE_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE -fsanitize=address)
endif()

if(USE_UBSAN)
    target_compile_options(${MAIN_EXECUTABLE_NAME} PRIVATE -fsanitize=undefined)
    target_link_options(${MAIN_EXECUTABLE_NAME} PRIVATE -fsanitize=undefined)
endif()
