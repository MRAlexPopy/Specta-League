cmake_minimum_required(VERSION 3.10)
project(Specta-League)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/artifacts/bin")


if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()


add_executable(oop
    main.cpp
    src/enemy.cpp
    src/enemyfactory.cpp
    src/hero.cpp
    src/strategy.cpp
)

target_include_directories(oop PRIVATE include)


if(EXISTS "${CMAKE_SOURCE_DIR}/tastatura.txt")
    file(COPY "${CMAKE_SOURCE_DIR}/tastatura.txt" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()
file(WRITE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/launcher.command" "#!/bin/bash\n./oop\n")