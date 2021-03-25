cmake_minimum_required(VERSION 3.17)
project(bebi C)

set(CMAKE_C_STANDARD 11)

include_directories(ex1)

add_executable(bebi
        ex1/main.c
        ex1/MyString.c
        ex1/MyStringUtils.c
        ex1/MyStringUnitTests.c
        ex1/MyString.h)
