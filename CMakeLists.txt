cmake_minimum_required(VERSION 3.17)
project(ex1 C)

set(CMAKE_C_STANDARD 99)

include_directories(ex1)

add_executable(ex1
        ex1/main.c
        ex1/MyString.c
        ex1/MyStringUtils.c
        ex1/MyStringTest.c
        ex1/MyStringUtils.h
        ex1/MyString.h)
