cmake_minimum_required (VERSION 2.4)
project (parseini)
include_directories (${PROJECT_SOURCE_DIR})
add_executable (pi main.c parseini.c)