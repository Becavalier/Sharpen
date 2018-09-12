cmake_minimum_required (VERSION 3.11)

project (Sharpen)

# set headers include path;
include_directories(.)

set (CMAKE_VERBOSE_MAKEFILE OFF)
set (CMAKE_CXX_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH dist)

# message (STATUS $ENV{CMAKE_TARGET})

# conditional compiling;
if ($ENV{CMAKE_TARGET} STREQUAL WASM) 
    set (OUTPUT sharpen.min)
    ADD_DEFINITIONS (-DWASM)
    set (CMAKE_CXX_FLAGS " \
    -O3 \
    -s WASM=1 \
    -s NO_EXIT_RUNTIME=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=1 \
    -s NO_FILESYSTEM=1 \
    -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"Pointer_stringify\",\"stringToUTF8\",\"lengthBytesUTF8\"] \
    ${CMAKE_CXX_FLAGS} \
    ")
else()
    set (OUTPUT sharpen)
    SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
endif()

# resolve source files;
aux_source_directory(src DIR_SRCS)

# add subdirectory;
add_subdirectory(lib/core/)
add_subdirectory(lib/vdom/)
add_subdirectory(lib/parser/)

# compiling target;
add_executable(${OUTPUT} ${DIR_SRCS})

# link libraries;
target_link_libraries(${OUTPUT} core parser vdom)
