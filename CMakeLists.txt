project(ctree)

cmake_minimum_required(VERSION 2.6)

add_definitions("-Wall -ansi -O2")

# libify
add_library(ctree SHARED ctree.c)

# make demo
add_executable(demo demo.c)
target_link_libraries(demo ctree)
