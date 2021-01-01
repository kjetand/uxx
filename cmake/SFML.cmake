if (MSVC)
    set(SFML_VERSION 2.5.1)
    set(SFML_URL https://www.sfml-dev.org/files/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip)

    if (NOT EXISTS ${CMAKE_BINARY_DIR}/SFML-${SFML_VERSION}/)
        message(STATUS "[uxx] Downloading SFML ${SFML_VERSION}")
        file(DOWNLOAD ${SFML_URL} ${CMAKE_BINARY_DIR}/SFML.zip
                TIMEOUT 60
                SHOW_PROGRESS)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/SFML.zip
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        file(REMOVE ${CMAKE_BINARY_DIR}/SFML.zip)
    else ()
        message(STATUS "[uxx] Using cached SFML")
    endif ()

    set(SFML_ROOT ${CMAKE_BINARY_DIR}/SFML-${SFML_VERSION}/)
    set(SFML_INCLUDE_DIR ${SFML_ROOT}/include/)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
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
    find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
endif ()
