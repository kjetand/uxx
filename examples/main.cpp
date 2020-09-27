#include <uxx/uxx.hpp>

#include <math.h>

static constexpr auto WHITE = uxx::rgba_color::from_integers(255, 255, 255, 255);
static constexpr auto BLACK = uxx::rgba_color::from_integers(0, 0, 0, 255);
static constexpr auto GREEN = uxx::rgba_color::from_integers(0, 255, 0, 255);
static constexpr auto RED = uxx::rgba_color::from_integers(255, 0, 0, 255);

struct canvas_state {
    std::vector<uxx::vec2d> points {};
    uxx::vec2d scrolling { 0.0f, 0.0f };
    bool adding_line = false;
    uxx::result<bool> enable_context_menu { true };
    uxx::result<bool> enable_grid { true };
};

static void draw_gradient(uxx::canvas& canvas, uxx::pencil& pencil, const uxx::rgba_color& color_a, const uxx::rgba_color& color_b)
{
    const auto p0 = canvas.get_position();
    const auto p1 = uxx::vec2d { p0.x + canvas.get_size().x, p0.y + canvas.get_size().y };
    pencil.draw_rect_filled_multi_color(p0, p1, { color_a, color_b, color_b, color_a });
}

static void show_canvas_popup(uxx::popup& popup, canvas_state& state)
{
    if (state.adding_line) {
        state.points.resize(state.points.size() - 2);
    }
    state.adding_line = false;

    if (popup.menu_item("Remove one", state.points.size() > 0)) {
        state.points.resize(state.points.size() - 2);
    }
    if (popup.menu_item("Remove all", state.points.size() > 0)) {
        state.points.clear();
    }
}

static void draw_canvas_grid(uxx::pencil& pencil, canvas_state& state, const uxx::vec2d& canvas_size, const uxx::vec2d& canvas_p0, const uxx::vec2d& canvas_p1, const uxx::vec2d& origin)
{
    if (state.enable_grid) {
        constexpr float GRID_STEP = 64.0f;
        constexpr auto color = uxx::rgba_color::from_integers(200, 200, 200, 40);
        pencil.set_color(color);

        for (float x = fmodf(state.scrolling.x, GRID_STEP); x < canvas_size.x; x += GRID_STEP) {
            pencil.draw_line({ canvas_p0.x + x, canvas_p0.y }, { canvas_p0.x + x, canvas_p1.y });
        }
        for (float y = fmodf(state.scrolling.y, GRID_STEP); y < canvas_size.y; y += GRID_STEP) {
            pencil.draw_line({ canvas_p0.x, canvas_p0.y + y }, { canvas_p1.x, canvas_p0.y + y });
        }
    }
    constexpr auto color = uxx::rgba_color::from_integers(255, 255, 0, 255);
    pencil.set_color(color);
    pencil.set_thickness(2.0f);

    for (std::size_t n = 0; n < state.points.size(); n += 2) {
        pencil.draw_line({ origin.x + state.points[n].x, origin.y + state.points[n].y },
            { origin.x + state.points[n + 1].x, origin.y + state.points[n + 1].y });
    }
}

static void draw_canvas(uxx::canvas& canvas, uxx::pencil& pencil, canvas_state& state, const uxx::vec2d& canvas_p1)
{
    auto mouse = canvas.get_mouse();
    const auto canvas_p0 = canvas.get_position();
    const uxx::vec2d origin { canvas_p0.x + state.scrolling.x, canvas_p0.y + state.scrolling.y };
    const uxx::vec2d mouse_pos_in_canvas { mouse.get_x() - origin.x, mouse.get_y() - origin.y };

    if (canvas.is_hovered() && !state.adding_line && mouse.is_clicked(uxx::mouse::button::left)) {
        state.points.push_back(mouse_pos_in_canvas);
        state.points.push_back(mouse_pos_in_canvas);
        state.adding_line = true;
    }
    if (state.adding_line) {
        state.points.back() = mouse_pos_in_canvas;
        if (!mouse.is_down(uxx::mouse::button::left)) {
            state.adding_line = false;
        }
    }
    const float mouse_threshold_for_pan = state.enable_context_menu ? -1.0f : 0.0f;

    if (canvas.is_active() && mouse.is_dragging(uxx::mouse::button::right, mouse_threshold_for_pan)) {
        state.scrolling.x += mouse.get_delta_x();
        state.scrolling.y += mouse.get_delta_y();
    }
    if (state.enable_context_menu) {
        canvas.popup("context", show_canvas_popup, state);
    }
    pencil.clip_rectangle(canvas.get_position(), canvas_p1, draw_canvas_grid, state, canvas.get_size(), canvas.get_position(), canvas_p1, origin);
}

static void show_canvas_tab(uxx::window& tab)
{
    static canvas_state state {};

    tab.checkbox("Enable grid", state.enable_grid);
    tab.checkbox("Enable context menu", state.enable_context_menu);
    tab.label("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

    const auto canvas_p0 = tab.get_cursor_screen_position();
    auto canvas_size = tab.get_content_size();

    if (canvas_size.x < 50.0f) {
        canvas_size.x = 50.0f;
    }
    if (canvas_size.y < 50.0f) {
        canvas_size.y = 50.0f;
    }
    const auto canvas_p1 = uxx::vec2d { canvas_p0.x + canvas_size.x, canvas_p0.y + canvas_size.y };
    auto pencil = tab.create_pencil();

    constexpr auto filled_color = uxx::rgba_color::from_integers(50, 50, 50, 255);
    pencil.set_color(filled_color);
    pencil.draw_rect_filled(canvas_p0, canvas_p1);
    pencil.set_color(BLACK);
    pencil.draw_rect(canvas_p0, canvas_p1);

    tab.canvas("canvas", canvas_size, draw_canvas, state, canvas_p1);
}

static void show_background_tab(uxx::window& tab)
{
    static uxx::result<bool> draw_bg { true };
    static uxx::result<bool> draw_fg { true };

    tab.checkbox("Draw in the background", draw_bg);
    tab.same_line();
    tab.checkbox("Draw in the foreground", draw_fg);
    tab.same_line();

    const auto window_pos = tab.get_position();
    const auto window_size = tab.get_size();
    const uxx::vec2d window_center { window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f };

    if (draw_bg) {
        auto pencil = tab.create_pencil_background();
        pencil.set_color(RED);
        pencil.set_thickness(14.f);
        pencil.draw_circle(window_center, uxx::radius { window_size.x * 0.6f });
    }
    if (draw_fg) {
        auto pencil = tab.create_pencil_foreground();
        pencil.set_color(GREEN);
        pencil.set_thickness(10.f);
        pencil.draw_circle(window_center, uxx::radius { window_size.y * 0.6f });
    }
}

static void show_primitives_tab(uxx::window& tab)
{
    tab.label("Gradients");
    const uxx::vec2d gradient_size { tab.get_content_size().x, 20.0f };
    tab.canvas("##gradient1", gradient_size, draw_gradient, WHITE, BLACK);
    tab.canvas("##gradient2", gradient_size, draw_gradient, GREEN, RED);

    auto pencil = tab.create_pencil();

    tab.label("All primitives");
    static uxx::result<float> sz { 36.0f };
    tab.slider_float("Size", sz, uxx::min<float>(2.0f), uxx::max<float>(72.0f));

    static uxx::result<float> thickness { 3.0f };
    tab.slider_float("Thickness", thickness, uxx::min<float>(1.0f), uxx::max<float>(8.0f));

    static uxx::result<int> ngon_sides { 6 };
    tab.slider_int("N-gon sides", ngon_sides, uxx::min<int> { 3 }, uxx::max<int> { 12 });

    static uxx::result<bool> circle_segments_override { false };
    static uxx::result<int> circle_segments_override_v { 12 };
    tab.checkbox("##circlesegmentoverride", circle_segments_override);
    tab.same_line();

    if (tab.slider_int("Circle segments", circle_segments_override_v, uxx::min<int> { 3 }, uxx::max<int> { 40 })) {
        circle_segments_override = true;
    }
    static uxx::result<uxx::rgba_color> col { uxx::rgba_color { 1.0f, 1.0f, 0.4f, 1.0f } };
    tab.color_picker("Color", col);

    const auto p = tab.get_cursor_screen_position();
    const auto spacing = 10.0f;
    const auto circle_segments = circle_segments_override ? circle_segments_override_v : 0;
    float x = p.x + 4.0f;
    float y = p.y + 4.0f;

    for (int n = 0; n < 2; n++) {
        const float th = (n == 0) ? 1.0f : thickness;

        pencil.set_color(col.get());
        pencil.set_thickness(th);
        pencil.draw_ngon({ x + sz * 0.5f, y + sz * 0.5f }, uxx::radius { sz * 0.5f }, ngon_sides);
        x += sz + spacing;
        pencil.draw_circle({ x + sz * 0.5f, y + sz * 0.5f }, uxx::radius { sz * 0.5f }, circle_segments);
        x += sz + spacing;
        pencil.set_corner_properties(uxx::pencil::corner_properties {});
        pencil.draw_rect({ x, y }, { x + sz, y + sz });
        x += sz + spacing;
        pencil.set_rounding(10.0f);
        pencil.set_corner_properties(uxx::pencil::corner_properties {}.set_all());
        pencil.draw_rect({ x, y }, { x + sz, y + sz });
        x += sz + spacing;
        pencil.set_corner_properties(uxx::pencil::corner_properties {}.set_top_left().set_bottom_right());
        pencil.draw_rect({ x, y }, { x + sz, y + sz });
        pencil.set_rounding(0);
        x += sz + spacing;
        pencil.draw_triangle({ x + sz * 0.5f, y }, { x + sz, y + sz - 0.5f }, { x, y + sz - 0.5f });
        x += sz + spacing;
        pencil.draw_line({ x, y }, { x + sz, y });
        x += sz + spacing;
        pencil.draw_line({ x, y }, { x, y + sz });
        x += spacing;
        pencil.draw_line({ x, y }, { x + sz, y + sz });
        x += sz + spacing;
        pencil.draw_bezier_curve({ x, y }, { x + sz * 1.3f, y + sz * 0.3f }, { x + sz - sz * 1.3f, y + sz - sz * 0.3f }, { x + sz, y + sz });
        x = p.x + 4;
        y += sz + spacing;
    }
    pencil.draw_ngon_filled({ x + sz * 0.5f, y + sz * 0.5f }, uxx::radius { sz * 0.5f }, ngon_sides);
    x += sz + spacing;
    pencil.draw_circle_filled({ x + sz * 0.5f, y + sz * 0.5f }, uxx::radius { sz * 0.5f }, circle_segments);
    x += sz + spacing;
    pencil.draw_rect_filled({ x, y }, { x + sz, y + sz });
    x += sz + spacing;
    pencil.set_rounding(10.0f);
    pencil.draw_rect_filled({ x, y }, { x + sz, y + sz });
    x += sz + spacing;
    pencil.set_corner_properties(uxx::pencil::corner_properties {}.set_top_left().set_bottom_right());
    pencil.draw_rect_filled({ x, y }, { x + sz, y + sz });
    pencil.set_rounding(0);
    x += sz + spacing;
    pencil.draw_triangle_filled({ x + sz * 0.5f, y }, { x + sz, y + sz - 0.5f }, { x, y + sz - 0.5f });
    x += sz + spacing;
    pencil.draw_rect_filled({ x, y }, { x + sz, y + thickness });
    x += sz + spacing;
    pencil.draw_rect_filled({ x, y }, { x + thickness, y + sz });
    x += spacing * 2.0f;
    pencil.draw_rect_filled({ x, y }, { x + 1, y + 1 });
    x += sz;

    constexpr uxx::color_rect colors { uxx::rgba_color::from_integers(0, 0, 0, 255),
        uxx::rgba_color::from_integers(255, 0, 0, 255),
        uxx::rgba_color::from_integers(255, 255, 0, 255),
        uxx::rgba_color::from_integers(0, 255, 0, 255) };
    pencil.draw_rect_filled_multi_color({ x, y }, { x + sz, y + sz }, colors);

    tab.empty_space({ (sz + spacing) * 8.8f, (sz + spacing) * 3.0f });
}

static void show_draw_primitives_window(uxx::scene& scene)
{
    scene.window("Draw primitives", [](auto& window) {
        window.tab_bar("##TabBar", [](auto& tab_bar) {
            tab_bar.item("Primitives", show_primitives_tab);
            tab_bar.item("Canvas", show_canvas_tab);
            tab_bar.item("Background/Foreground", show_background_tab);
        });
    });
}

static void show_style_sizes_tab(uxx::window& tab, uxx::style& style)
{
    tab.label("Main");
    tab.slider_float("WindowPadding", style.WindowPadding, uxx::min<float>(0.0f), uxx::max<float>(20.0f));
    tab.slider_float("FramePadding", style.FramePadding, uxx::min<float>(0.0f), uxx::max<float>(20.0f));
    tab.slider_float("ItemSpacing", style.ItemSpacing, uxx::min<float>(0.0f), uxx::max<float>(20.0f));
    tab.slider_float("ItemInnerSpacing", style.ItemInnerSpacing, uxx::min<float>(0.0f), uxx::max<float>(20.0f));
    tab.slider_float("TouchExtraPadding", style.TouchExtraPadding, uxx::min<float>(0.0f), uxx::max<float>(10.0f));
    tab.slider_float("IndentSpacing", style.IndentSpacing, uxx::min<float>(0.0f), uxx::max<float>(30.0f));
    tab.slider_float("ScrollbarSize", style.ScrollbarSize, uxx::min<float>(1.0f), uxx::max<float>(20.0f));
    tab.slider_float("GrabMinSize", style.GrabMinSize, uxx::min<float>(1.0f), uxx::max<float>(20.0f));

    tab.label("Borders");
    tab.slider_float("WindowBorderSize", style.WindowBorderSize, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.slider_float("ChildBorderSize", style.ChildBorderSize, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.slider_float("PopupBorderSize", style.PopupBorderSize, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.slider_float("FrameBorderSize", style.FrameBorderSize, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.slider_float("TabBorderSize", style.TabBorderSize, uxx::min<float>(0.0f), uxx::max<float>(1.0f));

    tab.label("Rounding");
    tab.slider_float("WindowRounding", style.WindowRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("ChildRounding", style.ChildRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("FrameRounding", style.FrameRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("PopupRounding", style.PopupRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("ScrollbarRounding", style.ScrollbarRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("GrabRounding", style.GrabRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("LogSliderDeadzone", style.LogSliderDeadzone, uxx::min<float>(0.0f), uxx::max<float>(12.0f));
    tab.slider_float("TabRounding", style.TabRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f));

    tab.label("Alignment");
    tab.slider_float("WindowTitleAlign", style.WindowTitleAlign, uxx::min<float>(0.0f), uxx::max<float>(1.0f));

    //int window_menu_button_position = style.WindowMenuButtonPosition + 1;

    //if (tab.combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0")) {
    //    style.WindowMenuButtonPosition = window_menu_button_position - 1;
    //}
    //tab.combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
    tab.slider_float("ButtonTextAlign", style.ButtonTextAlign, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.same_line();
    tab.slider_float("SelectableTextAlign", style.SelectableTextAlign, uxx::min<float>(0.0f), uxx::max<float>(1.0f));
    tab.same_line();
    tab.label("Safe Area Padding");
    tab.same_line();
    tab.slider_float("DisplaySafeAreaPadding", style.DisplaySafeAreaPadding, uxx::min<float>(0.0f), uxx::max<float>(30.0f));
}

static void show_style_colors_tab(uxx::window& tab)
{
    static uxx::result<bool> output_only_modified { true };
    tab.checkbox("Only Modified Colors", output_only_modified);

    //static ImGuiTextFilter filter;
    static uxx::color_picker_properties alpha_flags;

    //filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

    if (tab.radio_button("Opaque", alpha_flags == ImGuiColorEditFlags_None)) {
        alpha_flags.clear();
    }
    tab.same_line();

    if (tab.radio_button("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) {
        alpha_flags = uxx::color_picker_properties{}.set_alpha_preview();
    }
    tab.same_line();
    if (tab.radio_button("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) {
        alpha_flags = uxx::color_picker_properties{}.set_alpha_preview_half();
    }
    tab.same_line();

    ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
    //ImGui::PushItemWidth(-160);
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
        const char* name = ImGui::GetStyleColorName(i);
        if (!filter.PassFilter(name)) {
            continue;
        }
        ImGui::PushID(i);
        ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
        if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0) {
            // Tips: in a real user application, you may want to merge and use an icon font into the main font,
            // so instead of "Save"/"Revert" you'd use icons!
            // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
            ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
            if (ImGui::Button("Save")) {
                ref->Colors[i] = style.Colors[i];
            }
            ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
            if (ImGui::Button("Revert")) {
                style.Colors[i] = ref->Colors[i];
            }
        }
        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
        ImGui::TextUnformatted(name);
        ImGui::PopID();
    }
    //ImGui::PopItemWidth();
    ImGui::EndChild();
}

static void show_style_rendering_tooltip(uxx::tooltip& tooltip)
{
    const auto p = tooltip.get_position();
    auto pencil = tooltip.create_pencil();
    pencil.set_color(WHITE);

    constexpr float RAD_MIN = 10.0f;
    constexpr float RAD_MAX = 80.0f;

    float off_x = 10.0f;

    for (int n = 0; n < 7; n++) {
        const uxx::radius rad { RAD_MIN + (RAD_MAX - RAD_MIN) * static_cast<float>(n) / (7.0f - 1.0f) };
        pencil.draw_circle({ p.x + off_x + rad, p.y + RAD_MAX }, rad, 0);
        off_x += 10.0f + rad * 2.0f;
    }
    tooltip.empty_space({ off_x, RAD_MAX * 2.0f });
}

static void show_style_rendering_tab(uxx::window& tab, uxx::style& style)
{
    tab.checkbox("Anti-aliased lines", style.AntiAliasedLines);
    tab.same_line();
    tab.checkbox("Anti-aliased lines use texture", style.AntiAliasedLinesUseTex);
    tab.same_line();
    tab.checkbox("Anti-aliased fill", style.AntiAliasedFill);

    tab.align_width(
        100.0f, [](uxx::window& tab, auto& style) {
            tab.drag_float("Curve Tessellation Tolerance", style.CurveTessellationTol, uxx::drag_speed<float> { 0.02f }, uxx::min<float> { 0.10f }, uxx::max<float> { 10.0f });

            if (style.CurveTessellationTol < 0.10f) {
                style.CurveTessellationTol = 0.10f;
            }
            tab.drag_float("Circle Segment Max Error", style.CircleSegmentMaxError, uxx::drag_speed<float> { 0.01f }, uxx::min<float> { 0.10f }, uxx::max<float> { 10.0f });

            if (tab.is_item_active()) {
                //ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
                tab.tooltip(show_style_rendering_tooltip);
            }
            tab.same_line();

            tab.drag_float("Global Alpha", style.Alpha, uxx::drag_speed<float> { 0.005f }, uxx::min<float> { 0.20f }, uxx::max<float> { 1.0f });
        },
        style);
}

static void show_style_editor(uxx::scene& scene)
{
    scene.window("Style editor", [](uxx::window& window) {
        window.align_width(window.get_size().x * 0.5f, [](uxx::window& window) {
            static auto& style = uxx::app::get_style();

            if (window.slider_float("FrameRounding", style.FrameRounding, uxx::min<float>(0.0f), uxx::max<float>(12.0f)))
                style.GrabRounding = style.FrameRounding;
            {
                uxx::result<bool> border { style.WindowBorderSize > 0.0f };

                if (window.checkbox("WindowBorder", border)) {
                    style.WindowBorderSize = border ? 1.0f : 0.0f;
                }
            }
            window.same_line();
            {
                uxx::result<bool> border { style.FrameBorderSize > 0.0f };

                if (window.checkbox("FrameBorder", border)) {
                    style.FrameBorderSize = border ? 1.0f : 0.0f;
                }
            }
            window.same_line();
            {
                uxx::result<bool> border { style.PopupBorderSize > 0.0f };

                if (window.checkbox("PopupBorder", border)) {
                    style.PopupBorderSize = border ? 1.0f : 0.0f;
                }
            }
            window.separator();

            window.tab_bar(
                "##tabs", [](uxx::tab_bar& tab_bar) {
                    tab_bar.item("Sizes", show_style_sizes_tab, style);
                    tab_bar.item("Colors", show_style_colors_tab);
                    tab_bar.item("Rendering", show_style_rendering_tab, style);

                    //if (ImGui::BeginTabItem("Fonts")) {
                    //    ImGuiIO& io = ImGui::GetIO();
                    //    ImFontAtlas* atlas = io.Fonts;
                    //    HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
                    //    ImGui::PushItemWidth(120);
                    //    for (int i = 0; i < atlas->Fonts.Size; i++) {
                    //        ImFont* font = atlas->Fonts[i];
                    //        ImGui::PushID(font);
                    //        NodeFont(font);
                    //        ImGui::PopID();
                    //    }
                    //    if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight)) {
                    //        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    //        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
                    //        ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
                    //        ImGui::TreePop();
                    //    }

                    //    // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
                    //    // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
                    //    const float MIN_SCALE = 0.3f;
                    //    const float MAX_SCALE = 2.0f;
                    //    HelpMarker(
                    //        "Those are old settings provided for convenience.\n"
                    //        "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                    //        "rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
                    //        "Using those settings here will give you poor quality results.");
                    //    static float window_scale = 1.0f;
                    //    if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_ClampOnInput)) // Scale only this window
                    //        ImGui::SetWindowFontScale(window_scale);
                    //    ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_ClampOnInput); // Scale everything
                    //    ImGui::PopItemWidth();

                    //    ImGui::EndTabItem();
                    //}
                });
            uxx::app::set_style(style);
        });
    });
}

int main()
{
    uxx::app app;
    return app.run([](auto& scene) {
        show_draw_primitives_window(scene);
        show_style_editor(scene);
    });
}