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

add_library(uxx_warnings INTERFACE)

find_package(OpenGL REQUIRED)

if (MSVC)
    set(SFML_URL https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip)
else ()
    set(SFML_URL https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/SFML-2.5.1/)
    message(STATUS "[uxx] Downloading SFML 2.5.1")
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
    target_compile_options(uxx_warnings INTERFACE /W4 /WX "/permissive-")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "[uxx] Debug build for Windows")
        set(SFML_LIBRARIES
                ${SFML_ROOT}/lib/sfml-graphics-d.lib
                ${SFML_ROOT}/lib/sfml-system-d.lib
                ${SFML_ROOT}/lib/sfml-window-d.lib)
        message(STATUS "[uxx] Copying SFML debug DLLs to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        file(COPY ${SFML_ROOT}/bin/sfml-graphics-d-2.dll
                ${SFML_ROOT}/bin/sfml-system-d-2.dll
                ${SFML_ROOT}/bin/sfml-window-d-2.dll
                DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else ()
        message(STATUS "[uxx] Release build for Windows")
        set(SFML_LIBRARIES
                ${SFML_ROOT}/lib/sfml-graphics.lib
                ${SFML_ROOT}/lib/sfml-system.lib
                ${SFML_ROOT}/lib/sfml-window.lib)
        message(STATUS "[uxx] Copying SFML release DLLs to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        file(COPY ${SFML_ROOT}/bin/sfml-graphics-2.dll
                ${SFML_ROOT}/bin/sfml-system-2.dll
                ${SFML_ROOT}/bin/sfml-window-2.dll
                DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif ()
else ()
    message(STATUS "[uxx] Building for Linux")
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

    set(SFML_LIBRARIES
            ${SFML_ROOT}/lib/libsfml-graphics.so
            ${SFML_ROOT}/lib/libsfml-system.so
            ${SFML_ROOT}/lib/libsfml-window.so)
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
