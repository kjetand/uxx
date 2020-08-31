add_executable(unit-tests string_ref_test.cpp)

target_include_directories(unit-tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/ext/include/)

target_link_libraries(unit-tests PRIVATE project_warnings ${PROJECT_NAME})

add_test(run-unit-tests unit-tests)