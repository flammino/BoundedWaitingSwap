cmake_minimum_required(VERSION 3.10)
project(flamminoWeek6 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(flamminoWeek6 main.c)