#include <uxx/uxx.hpp>

#include <math.h>

static void show_draw_primitives_window(uxx::scene& scene)
{
    scene.window("Draw primitives", [](auto& w) {
        w.tab_bar("##TabBar", [](auto& tb) {
            tb.item("Primitives", [](auto&) {
                // TODO: Port ImGui demo code here
            });

            tb.item("Canvas", [](auto& tab) {
                static std::vector<uxx::vec2d> points;
                static uxx::vec2d scrolling { 0.0f, 0.0f };
                static bool opt_enable_grid = true;
                static bool opt_enable_context_menu = true;
                static bool adding_line = false;

                tab.check_box("Enable grid", opt_enable_grid);
                tab.check_box("Enable context menu", opt_enable_context_menu);
                tab.label("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

                const auto canvas_p0 = tab.get_cursor_screen_position();
                auto canvas_sz = tab.get_available_content_region();

                if (canvas_sz.x < 50.0f) {
                    canvas_sz.x = 50.0f;
                }
                if (canvas_sz.y < 50.0f) {
                    canvas_sz.y = 50.0f;
                }
                const auto canvas_p1 = uxx::vec2d { canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y };

                // Draw border and background color
                auto pencil = tab.create_pencil();
                pencil.set_color(uxx::rgba_color::from_integers(50, 50, 50, 255));
                pencil.draw_rect_filled(canvas_p0, canvas_p1);
                pencil.set_color(uxx::rgba_color::from_integers(255, 255, 255, 255));
                pencil.draw_rect(canvas_p0, canvas_p1);

                auto mouse = tab.get_mouse();

                // This will catch our interactions
                tab.invisible_button("canvas", canvas_sz);
                const bool is_hovered = tab.is_item_hovered();
                const bool is_active = tab.is_item_active();
                const uxx::vec2d origin { canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y };
                const uxx::vec2d mouse_pos_in_canvas { mouse.get_x() - origin.x, mouse.get_y() - origin.y };

                // Add first and second point
                if (is_hovered && !adding_line && mouse.is_clicked(uxx::mouse::button::left)) {
                    points.push_back(mouse_pos_in_canvas);
                    points.push_back(mouse_pos_in_canvas);
                    adding_line = true;
                }
                if (adding_line) {
                    points.back() = mouse_pos_in_canvas;
                    if (!mouse.is_down(uxx::mouse::button::left)) {
                        adding_line = false;
                    }
                }

                // Pan (we use a zero mouse threshold when there's no context menu)
                // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
                // TODO: Should lock_threshold be part of the mouse object?
                const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
                if (is_active && mouse.is_dragging(uxx::mouse::button::right, mouse_threshold_for_pan)) {
                    scrolling.x += mouse.get_delta_x();
                    scrolling.y += mouse.get_delta_y();
                }

                // Context menu (under default mouse threshold)
                const auto drag_delta = mouse.get_drag_delta(uxx::mouse::button::right);
                if (opt_enable_context_menu && mouse.is_released(uxx::mouse::button::right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
                    tab.open_popup_context_item("context");
                }
                tab.popup("context", [](auto& pu) {
                    if (adding_line) {
                        points.resize(points.size() - 2);
                    }
                    adding_line = false;

                    if (pu.menu_item("Remove one", points.size() > 0)) {
                        points.resize(points.size() - 2);
                    }
                    if (pu.menu_item("Remove all", points.size() > 0)) {
                        points.clear();
                    }
                });

                // Draw grid + all lines in the canvas
                // TODO: move to pencil!
                tab.clip_rect(
                    canvas_p0, canvas_p1, true, [](auto& rect, auto& canvas_sz, auto& canvas_p0, auto& canvas_p1, auto& origin) {
                        auto pencil = rect.create_pencil();

                        if (opt_enable_grid) {
                            constexpr float GRID_STEP = 64.0f;
                            pencil.set_color(uxx::rgba_color::from_integers(200, 200, 200, 40));

                            for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP) {
                                pencil.draw_line(uxx::vec2d { canvas_p0.x + x, canvas_p0.y }, uxx::vec2d { canvas_p0.x + x, canvas_p1.y });
                            }
                            for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP) {
                                pencil.draw_line(uxx::vec2d { canvas_p0.x, canvas_p0.y + y }, uxx::vec2d { canvas_p1.x, canvas_p0.y + y });
                            }
                        }
                        pencil.set_color(uxx::rgba_color::from_integers(255, 255, 0, 255));
                        pencil.set_thickness(2.0f);
                        for (std::size_t n = 0; n < points.size(); n += 2) {
                            pencil.draw_line(uxx::vec2d { origin.x + points[n].x, origin.y + points[n].y },
                                uxx::vec2d { origin.x + points[n + 1].x, origin.y + points[n + 1].y });
                        }
                    },
                    canvas_sz, canvas_p0, canvas_p1, origin);
            });

            tb.item("Background/Foreground", [](auto& tab) {
                static bool draw_bg = true;
                static bool draw_fg = true;

                tab.check_box("Draw in the background", draw_bg);
                tab.same_line();
                tab.check_box("Draw in the foreground", draw_fg);
                tab.same_line();

                const auto window_pos = tab.get_position();
                const auto window_size = tab.get_size();
                const auto window_center = uxx::vec2d { window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f };

                if (draw_bg) {
                    auto pencil = tab.create_pencil(uxx::pencil::type::background);
                    pencil.set_color(uxx::rgba_color::from_integers(255, 0, 0, 200));
                    pencil.set_thickness(14.f);
                    pencil.draw_circle(window_center, window_size.x * 0.6f);
                }
                if (draw_fg) {
                    auto pencil = tab.create_pencil(uxx::pencil::type::foreground);
                    pencil.set_color(uxx::rgba_color::from_integers(0, 255, 0, 200));
                    pencil.set_thickness(10.f);
                    pencil.draw_circle(window_center, window_size.y * 0.6f);
                }
            });
        });
    });
}

int main()
{
    uxx::app app;
    return app.run([](auto& scene, auto title) {
        show_draw_primitives_window(scene);

        static bool open = true;
        static const auto properties = uxx::window::properties {};

        scene.window(title, open, properties, [](auto& w) {
            if (w.is_collapsed()) {
                return;
            }
            w.label("Enter your name: ");
            w.same_line();

            static std::string name {};
            w.input_text("", name);

            if (w.button("Submit")) {
                std::puts("Your name is: ");
                std::puts(name.c_str());
            }
        });
    },
        "Hello, world!");
}