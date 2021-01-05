find_program(DOCKER_EXE Docker)

if (DOCKER_EXE)
    add_custom_target(gcc-10 COMMAND ${DOCKER_EXE} build -f Dockerfile.gcc10 -t ${PROJECT_NAME}:gcc10 . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    add_custom_target(clang-10 COMMAND ${DOCKER_EXE} build -f Dockerfile.clang10 -t ${PROJECT_NAME}:clang10 . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    add_custom_target(build-all DEPENDS gcc-10 clang-10)
endif ()