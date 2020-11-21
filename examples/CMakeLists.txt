add_executable(${PROJECT_NAME}_graphics_demo graphics_demo.cpp)

target_link_libraries(${PROJECT_NAME}_graphics_demo PRIVATE
        uxx_warnings
        ${PROJECT_NAME}
        ${SFML_LIBRARIES})

target_include_directories(${PROJECT_NAME}_graphics_demo PRIVATE
        ${PROJECT_SOURCE_DIR}/include)

file(GLOB IMAGE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/*)
file(COPY ${IMAGE_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
