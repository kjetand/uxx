add_library(imgui-sfml SHARED
        imgui_widgets.cpp
        imgui_draw.cpp
        imgui-SFML.cpp
        imgui.cpp)

target_include_directories(imgui-sfml PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SFML_INCLUDE_DIR})

target_link_libraries(imgui-sfml PRIVATE
        ${OPENGL_LIBRARIES}
        ${SFML_LIBRARIES})