find_package(Doxygen)

if (DOXYGEN_FOUND)
    add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "[uxx] Generating API documentation with Doxygen"
            MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/Doxyfile
            VERBATIM)
else (DOXYGEN_FOUND)
    message(STATUS "[uxx] Doxygen not found")
endif (DOXYGEN_FOUND)