add_library(${PROJECT_NAME} SHARED uxx.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
