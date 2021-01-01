if (MSVC)
    set(LIBVLC_VERSION 3.0.12.1)
    set(LIBVLC_DOWNLOAD_FILENAME LibVLC.7z)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBVLC_URL https://artifacts.videolan.org/vlc-3.0/nightly-win64/20210101-0621/vlc-${LIBVLC_VERSION}-win64-debug-8e19ecd0.7z)
    else ()
        set(LIBVLC_URL https://artifacts.videolan.org/vlc-3.0/nightly-win64/20210101-0621/vlc-${LIBVLC_VERSION}-win64-8e19ecd0.7z)
    endif ()

    if (NOT EXISTS ${CMAKE_BINARY_DIR}/vlc-${LIBVLC_VERSION}/)
        message(STATUS "[uxx] Downloading LibVLC")
        file(DOWNLOAD ${LIBVLC_URL} ${CMAKE_BINARY_DIR}/${LIBVLC_DOWNLOAD_FILENAME}
                TIMEOUT 60
                SHOW_PROGRESS)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/${LIBVLC_DOWNLOAD_FILENAME}
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        file(REMOVE ${CMAKE_BINARY_DIR}/${LIBVLC_DOWNLOAD_FILENAME})
    else ()
        message(STATUS "[uxx] Using cached LibVLC")
    endif ()

    set(LIBVLC_ROOT ${CMAKE_BINARY_DIR}/vlc-${LIBVLC_VERSION}/)
    set(LIBVLC_INCLUDE_DIR ${LIBVLC_ROOT}/sdk/include/)

    set(LIBVLC_LIBRARIES
            ${LIBVLC_ROOT}/sdk/lib/libvlc.lib
            ${LIBVLC_ROOT}/sdk/lib/libvlccore.lib)
    file(COPY
            ${LIBVLC_ROOT}/libvlc.dll
            ${LIBVLC_ROOT}/libvlccore.dll
            DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${LIBVLC_ROOT}/plugins DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else ()
    find_path(LIBVLC_INCLUDE_DIR vlc/vlc.h
            HINTS "$ENV{LIBVLC_INCLUDE_PATH}"
            PATHS "/usr/include" "/usr/include/vlc")

    find_path(LIBVLC_INCLUDE_DIR PATHS "${CMAKE_INCLUDE_PATH}/vlc" NAMES vlc.h)

    find_library(LIBVLC_LIBRARIES NAMES libvlc
            HINTS "$ENV{LIBVLC_LIBRARIES_PATH}"
            PATHS
            "$ENV{LIB_DIR}/lib")

    find_library(LIBVLC_LIBRARIES NAMES vlc libvlc)

    find_library(LIBVLCCORE_LIBRARY NAMES vlccore libvlccore
            HINTS "$ENV{LIBVLC_LIBRARIES_PATH}"
            PATHS
            "$ENV{LIB_DIR}/lib")

    find_library(LIBVLCCORE_LIBRARY NAMES vlccore libvlccore)

    if (LIBVLC_INCLUDE_DIR AND LIBVLC_LIBRARIES AND LIBVLCCORE_LIBRARY)
        set(LIBVLC_FOUND TRUE)
    endif (LIBVLC_INCLUDE_DIR AND LIBVLC_LIBRARIES AND LIBVLCCORE_LIBRARY)

    if (LIBVLC_FOUND)
        get_filename_component(LIBVLC_LIB_DIR ${LIBVLC_LIBRARIES} PATH)
        message(STATUS "[uxx] Found LibVLC")
    else ()
        message(FATAL_ERROR "[uxx] Could not find LibVLC")
    endif ()
endif ()
