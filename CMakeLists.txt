cmake_minimum_required(VERSION 3.26)
project(shell C)

set(CMAKE_C_STANDARD 99)

add_executable(shell main.c
        main.h)
