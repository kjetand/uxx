cmake_minimum_required(VERSION 3.15)
project(uxx CXX)

if (APPLE)
    message(FATAL_ERROR "OSX is currently not supported")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "Only MSVC and GCC compilers are currently supported")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_warnings INTERFACE)

find_package(OpenGL REQUIRED)

if (MSVC)
    set(SFML_URL https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip)
else ()
    set(SFML_URL https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/SFML-2.5.1/)
    message(STATUS "Downloading SFML 2.5.1")
    file(DOWNLOAD ${SFML_URL} ${CMAKE_BINARY_DIR}/SFML.zip
            TIMEOUT 60
            SHOW_PROGRESS)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/SFML.zip
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    file(REMOVE ${CMAKE_BINARY_DIR}/SFML.zip)
endif ()

set(SFML_ROOT ${CMAKE_BINARY_DIR}/SFML-2.5.1/)
set(SFML_INCLUDE_DIR ${SFML_ROOT}/include/)

if (MSVC)
    message(STATUS "Building for Windows")
    target_compile_options(project_warnings INTERFACE /W4 /WX "/permissive-")

    set(SFML_LIBRARIES
            ${SFML_ROOT}/lib/sfml-graphics.lib
            ${SFML_ROOT}/lib/sfml-system.lib
            ${SFML_ROOT}/lib/sfml-window.lib)
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

    set(SFML_LIBRARIES
            ${SFML_ROOT}/lib/libsfml-graphics.so
            ${SFML_ROOT}/lib/libsfml-system.so
            ${SFML_ROOT}/lib/libsfml-window.so)
endif ()

message(STATUS "SFML include directory: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")

add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(examples)

enable_testing()
add_subdirectory(test)
