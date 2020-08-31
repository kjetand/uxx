add_executable(${PROJECT_NAME}-example main.cpp)

target_link_libraries(${PROJECT_NAME}-example PRIVATE
        project_warnings
        ${PROJECT_NAME}
        ${SFML_LIBRARIES})

target_include_directories(${PROJECT_NAME}-example PRIVATE
        ${PROJECT_SOURCE_DIR}/include)
