add_definitions("-DBUILDING_UXX")

add_library(${PROJECT_NAME} SHARED uxx.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings ${SFML_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/SFML-2.5.1/include
        ${PROJECT_SOURCE_DIR}/ext/include)
