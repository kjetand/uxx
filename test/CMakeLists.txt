add_executable(unit_tests string_ref_test.cpp)
target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/ext/Catch2/single_include)
target_link_libraries(unit_tests PRIVATE project_warnings ${PROJECT_NAME})

add_test(run_unit_tests unit_tests)