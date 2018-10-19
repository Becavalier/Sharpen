cmake_minimum_required (VERSION 3.11)

project (Sharpen)

# set headers include path;
include_directories(.)

set (CMAKE_VERBOSE_MAKEFILE "OFF")
set (CMAKE_CXX_STANDARD "11")
set (STATE $ENV{CMAKE_TARGET})

# set folders;
set (N_SOURCE_SRC "src")
set (N_OUTPUT_SRC "dist")
set (T_SOURCE_SRC "test/cpp")
set (T_OUTPUT_SRC "test/cpp")

# conditional compiling;
if (STATE STREQUAL WASM) 
    set (OUTPUT "sharpen.min")
    ADD_DEFINITIONS (-DWASM)
    set (CMAKE_CXX_FLAGS " \
    -O3 \
    -s WASM=1 \
    --closure 1 \
    --llvm-lto 1 \
    -fno-rtti \
    -s NO_EXIT_RUNTIME=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=0 \
    -s NO_FILESYSTEM=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=\"SharpenWasm\" \
    -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"Pointer_stringify\",\"stringToUTF8\",\"lengthBytesUTF8\"] \
    ${CMAKE_CXX_FLAGS} \
    ")
else()
    set (OUTPUT "sharpen")
    set (CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
endif()

if (STATE STREQUAL TEST)
    set (EXECUTABLE_OUTPUT_PATH ${T_OUTPUT_SRC})
    # download and unpack googletest at configure time
    configure_file (CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process (COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if (result)
        message (FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process (COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if (result)
        message (FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker settings on Windows;
    set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory (${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
    # resolve source files;
    aux_source_directory (${T_SOURCE_SRC} DIR_SRCS)
else()
    set (EXECUTABLE_OUTPUT_PATH ${N_OUTPUT_SRC})
    # resolve source files;
    aux_source_directory (${N_SOURCE_SRC} DIR_SRCS)
endif()                 

# add subdirectory;
add_subdirectory (lib/core/)
add_subdirectory (lib/vdom/)
add_subdirectory (lib/parser/)

# compiling target;
add_executable (${OUTPUT} ${DIR_SRCS})

# link libraries;
if (STATE STREQUAL TEST) 
    target_link_libraries (${OUTPUT} core parser vdom gtest_main gtest)
    # run the test;
    add_custom_command(TARGET ${OUTPUT} POST_BUILD
        COMMAND ${OUTPUT})
else()
    target_link_libraries (${OUTPUT} core parser vdom)
endif()  
