project(ctree)

cmake_minimum_required(VERSION 2.6)

add_definitions("-Wall -ansi -O2")

set(hdrs ctree.h)
set(srcs ctree.c)

# libify
set(libraryname ctree)
set(lib_major 0)
set(lib_minor 0)
set(lib_release 1)

add_library(${libraryname} SHARED ${hdrs} ${srcs})
set(_soversion ${lib_major}.${lib_minor}.${lib_release})
set_target_properties(${libraryname} PROPERTIES soversion ${lib_major} version ${_soversion})
target_link_libraries(${libraryname})
set(library_output_path ${project_binary_dir}/lib)

install(FILES ${hdrs} DESTINATION /usr/include/)
install(TARGETS ${libraryname} LIBRARY DESTINATION /usr/lib)
