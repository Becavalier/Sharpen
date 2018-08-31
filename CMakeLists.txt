cmake_minimum_required (VERSION 2.8)

project (Wsdom)

set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH dist)

# resolve source files;
aux_source_directory(src DIR_SRCS)

# add subdirectory;
add_subdirectory(lib/core/)
add_subdirectory(lib/vdom/)

# compiling target;
add_executable(output ${DIR_SRCS})

# link libraries;
target_link_libraries(output core)
