cmake_minimum_required(VERSION 3.17)
project(game_of_life C)

set(CMAKE_C_STANDARD 11)

add_executable(game_of_life main.c canvas.h canvas.c)