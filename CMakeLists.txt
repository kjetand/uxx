cmake_minimum_required(VERSION 3.15)
project(uxx CXX)

if (APPLE)
    message(FATAL_ERROR "[uxx] OSX is currently not supported")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
        NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND
        NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "[uxx] Only MSVC, GCC and Clang compilers are currently supported")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if (MSVC)
    message(STATUS "[uxx] Building for Windows")
else ()
    message(STATUS "[uxx] Building for Linux")
endif ()


add_library(uxx_warnings INTERFACE)
include(Dependencies)

if (MSVC)
    target_compile_options(uxx_warnings INTERFACE /W4 /WX "/permissive-")
else ()
    target_compile_options(uxx_warnings INTERFACE
            -Wall
            -Werror
            -Wextra
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
        target_compile_options(uxx_warnings INTERFACE
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast)
    endif ()
endif ()

message(STATUS "[uxx] Copying fonts to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(GLOB FONT_FILES ${PROJECT_SOURCE_DIR}/fonts/*.ttf)
file(COPY ${FONT_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

message(STATUS "[uxx] SFML include directory: ${SFML_INCLUDE_DIR}")
message(STATUS "[uxx] SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "[uxx] OpenGL libraries: ${OPENGL_LIBRARIES}")

add_subdirectory(ext)
add_subdirectory(src)
if (NOT UXX_DISABLE_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (NOT UXX_DISABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
