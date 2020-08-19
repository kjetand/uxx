cmake_minimum_required(VERSION 3.15)
project(uxx CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_warnings INTERFACE)

if (MSVC)
    message(STATUS "Building for Windows")
    target_compile_options(project_warnings INTERFACE /W4 /WX "/permissive-")
else ()
    message(STATUS "Building for Linux")
    target_compile_options(project_warnings INTERFACE
            -Wall
            -Werror
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(project_warnings INTERFACE
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast)
    endif ()
endif ()

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(examples)

enable_testing()
add_subdirectory(test)
