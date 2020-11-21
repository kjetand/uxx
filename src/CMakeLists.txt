add_definitions(-DBUILDING_UXX)

add_library(${PROJECT_NAME} SHARED
        string_ref.cpp
        app.cpp
        window.cpp
        pencil.cpp
        tab_bar.cpp
        mouse.cpp
        popup.cpp
        canvas.cpp
        scene.cpp
        menu_bar.cpp
        menu.cpp
        image.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        uxx_warnings
        ${OPENGL_LIBRARIES}
        ${SFML_LIBRARIES}
        imgui_sfml)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/SFML-2.5.1/include
        ${PROJECT_SOURCE_DIR}/ext/imgui
        ${PROJECT_SOURCE_DIR}/ext/include)
