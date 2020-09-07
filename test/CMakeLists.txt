add_executable(unit_tests string_ref_test.cpp)

target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/ext/include/)

target_link_libraries(unit_tests PRIVATE project_warnings ${PROJECT_NAME})

add_test(run_unit_tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests)