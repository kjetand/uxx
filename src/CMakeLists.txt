add_definitions(-DBUILDING_UXX)

add_library(${PROJECT_NAME} SHARED
        string_ref.cpp
        app.cpp
        pane.cpp
        pencil.cpp
        tab_bar.cpp
        mouse.cpp
        popup.cpp
        canvas.cpp
        scene.cpp
        menu_bar.cpp
        menu.cpp
        image.cpp
        video.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
        uxx_warnings
        ${OPENGL_LIBRARIES}
        ${SFML_LIBRARIES}
        ${LIBVLC_LIBRARIES}
        imgui_sfml)

if (MSVC)
    set(SFML_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SFML-2.5.1/include)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${SFML_INCLUDE_DIR}
        ${LIBVLC_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/ext/imgui
        ${PROJECT_SOURCE_DIR}/ext/include)
