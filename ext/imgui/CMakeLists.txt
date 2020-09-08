add_library(imgui_sfml STATIC
        imgui_widgets.cpp
        imgui_draw.cpp
        imgui-SFML.cpp
        imgui.cpp)

target_include_directories(imgui_sfml PRIVATE
        ${SFML_INCLUDE_DIR})

target_link_libraries(imgui_sfml PRIVATE
        ${OPENGL_LIBRARIES}
        ${SFML_LIBRARIES})