project(ctree)

cmake_minimum_required(VERSION 2.6)

if(APPLE)
    include_directories(/usr/include/malloc/)
endif(APPLE)

# libify
add_library(ctree SHARED ctree.c)

# make demo
add_executable(demo demo.c)
target_link_libraries(demo ctree)
