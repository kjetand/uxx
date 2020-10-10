add_executable(${PROJECT_NAME}_example main.cpp)

target_link_libraries(${PROJECT_NAME}_example PRIVATE
        uxx_warnings
        ${PROJECT_NAME}
        ${SFML_LIBRARIES})

target_include_directories(${PROJECT_NAME}_example PRIVATE
        ${PROJECT_SOURCE_DIR}/include)
