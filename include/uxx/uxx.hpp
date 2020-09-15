#ifndef _UXX_HPP
#define _UXX_HPP

#include <any>
#include <concepts>
#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#ifdef _WIN32
#ifdef BUILDING_UXX
#define UXX_EXPORT __declspec(dllexport)
#else
#define UXX_EXPORT __declspec(dllimport)
#endif
#else
#define UXX_EXPORT __attribute__((visibility("default")))
#endif

namespace uxx {

class window;

class UXX_EXPORT string_ref {
public:
    string_ref() = delete;
    constexpr string_ref(const char* str) noexcept // NOLINT
        : _str(str)
    {
    }

    string_ref(const std::string& str) noexcept; // NOLINT
    ~string_ref() noexcept = default;

    constexpr string_ref(const string_ref&) noexcept = default;
    constexpr string_ref(string_ref&&) noexcept = default;
    constexpr string_ref& operator=(const string_ref&) noexcept = default;
    constexpr string_ref& operator=(string_ref&&) noexcept = default;

    [[nodiscard]] constexpr operator const char*() const noexcept // NOLINT
    {
        return _str;
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept
    {
        return _str;
    }

private:
    const char* _str;
};

template <typename F, typename... Args>
concept function = std::is_invocable_v<F, Args...>;

struct vec2d {
    float x;
    float y;
};

using color32 = unsigned int;

struct UXX_EXPORT rgb_color {
    float r;
    float g;
    float b;

    [[nodiscard]] color32 to_color32() const noexcept;
};

struct UXX_EXPORT rgba_color {
    float r;
    float g;
    float b;
    float a;

    [[nodiscard]] color32 to_color32() const noexcept;

    [[nodiscard]] static rgba_color from_integers(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept;
};

[[nodiscard]] rgba_color to_rgba_color(const color32 c) noexcept;

struct color_rect {
    rgba_color upper_left;
    rgba_color upper_right;
    rgba_color bottom_right;
    rgba_color bottom_left;
};

class pencil {
    friend class window;

public:
    enum type {
        window, // Default
        background,
        foreground
    };
    class UXX_EXPORT properties {
    public:
        explicit properties() noexcept;
        ~properties() noexcept = default;

        properties(const properties&) = default;
        properties(properties&&) noexcept = default;
        properties& operator=(const properties&) = default;
        properties& operator=(properties&&) noexcept = default;

        [[nodiscard]] explicit operator int() const noexcept;

        properties clear() noexcept;
        properties set_anti_aliased_lines() noexcept;
        properties set_anti_aliased_lines_using_textures() noexcept;
        properties set_anti_aliased_fill() noexcept;

    private:
        int _flags;
    };

    class UXX_EXPORT corner_properties {
    public:
        explicit corner_properties() noexcept;
        ~corner_properties() noexcept = default;

        corner_properties(const corner_properties&) = default;
        corner_properties(corner_properties&&) noexcept = default;
        corner_properties& operator=(const corner_properties&) = default;
        corner_properties& operator=(corner_properties&&) noexcept = default;

        [[nodiscard]] explicit operator int() const noexcept;

        corner_properties clear() noexcept;
        corner_properties set_top_left() noexcept;
        corner_properties set_top_right() noexcept;
        corner_properties set_bottom_left() noexcept;
        corner_properties set_bottom_right() noexcept;
        corner_properties set_all() noexcept;

    private:
        int _flags;
    };

    UXX_EXPORT ~pencil() noexcept = default;

    UXX_EXPORT void set_color(const rgb_color& color) noexcept;
    UXX_EXPORT void set_color(const rgba_color& color) noexcept;
    UXX_EXPORT void set_thickness(float thickness) noexcept;
    UXX_EXPORT void set_rounding(float rounding) noexcept;
    UXX_EXPORT void set_properties(const properties& props) noexcept;
    UXX_EXPORT void set_corner_properties(const corner_properties& corner_props) noexcept;

    UXX_EXPORT void draw_line(const vec2d& from, const vec2d& to) const;
    UXX_EXPORT void draw_rect(const vec2d& min, const vec2d& max) const;
    UXX_EXPORT void draw_rect_filled(const vec2d& min, const vec2d& max) const;
    UXX_EXPORT void draw_rect_filled_multi_color(const vec2d& min, const vec2d& max, const color_rect& colors) const;
    UXX_EXPORT void draw_quad(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_quad_filled(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_triangle(const vec2d& p1, const vec2d& p2, const vec2d& p3) const;
    UXX_EXPORT void draw_triangle_filled(const vec2d& p1, const vec2d& p2, const vec2d& p3) const;
    UXX_EXPORT void draw_circle(const vec2d& center, float radius) const;
    UXX_EXPORT void draw_circle(const vec2d& center, float radius, int num_segments) const;
    UXX_EXPORT void draw_circle_filled(const vec2d& center, float radius) const;
    UXX_EXPORT void draw_circle_filled(const vec2d& center, float radius, int num_segments) const;
    UXX_EXPORT void draw_ngon(const vec2d& center, float radius, int num_segments) const;
    UXX_EXPORT void draw_ngon_filled(const vec2d& center, float radius, int num_segments) const;
    UXX_EXPORT void draw_polyline(const std::vector<vec2d>& points, bool closed) const;
    UXX_EXPORT void draw_convex_poly_filled(const std::vector<vec2d>& points) const;
    UXX_EXPORT void draw_bezier_curve(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_bezier_curve(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4, int num_segments) const;

    // TODO: Draw text, images...

private:
    std::any _draw_list;
    unsigned int _color;
    float _thickness;
    float _rounding;
    corner_properties _corner_props;

    explicit pencil() noexcept;
    explicit pencil(type pencil_type) noexcept;
};

class UXX_EXPORT tab_bar {
    friend class window;

public:
    tab_bar(const tab_bar&) = delete;
    tab_bar(tab_bar&&) noexcept = default;
    tab_bar& operator=(const tab_bar&) = delete;
    tab_bar& operator=(tab_bar&&) noexcept = delete;

    // TODO: Implement tab bar/item properties

    template <typename F, typename... Args>
    void item(string_ref label, F&& f, Args&&... args) const requires function<F, uxx::window&, Args...>
    {
        if (begin_tab_item(label)) {
            f(_window, std::forward<Args>(args)...);
            end_tab_item();
        }
    }

private:
    enum visible {
        yes,
        no
    };

    uxx::window& _window;

    explicit tab_bar(uxx::window& w) noexcept;
    ~tab_bar() noexcept = default;

    [[nodiscard]] bool begin_tab_item(string_ref label) const;
    void end_tab_item() const;
};

class UXX_EXPORT popup {
    friend class window;

public:
    enum class visible {
        yes,
        no
    };
    ~popup() noexcept = default;

    popup(const popup&) = delete;
    popup(popup&&) noexcept = default;
    popup& operator=(const popup&) = delete;
    popup& operator=(popup&&) noexcept = default;

    bool menu_item(string_ref label, bool enabled) const;

private:
    explicit popup() noexcept = default;
};

class mouse {
    friend class window;

public:
    enum class button {
        left,
        right,
        middle
    };
    ~mouse() noexcept = default;

    mouse(const mouse&) = delete;
    mouse(mouse&&) noexcept = default;
    mouse& operator=(const mouse&) = delete;
    mouse& operator=(mouse&&) noexcept = default;

    [[nodiscard]] UXX_EXPORT float get_x() const;
    [[nodiscard]] UXX_EXPORT float get_y() const;
    [[nodiscard]] UXX_EXPORT float get_delta_x() const;
    [[nodiscard]] UXX_EXPORT float get_delta_y() const;
    [[nodiscard]] UXX_EXPORT vec2d get_drag_delta(button b) const;

    [[nodiscard]] UXX_EXPORT bool is_clicked(button b) const noexcept;
    [[nodiscard]] UXX_EXPORT bool is_down(button b) const noexcept;
    [[nodiscard]] UXX_EXPORT bool is_released(button b) const noexcept;
    [[nodiscard]] UXX_EXPORT bool is_dragging(button b, float lock_threshold) const noexcept;

private:
    std::any _io;

    explicit mouse() noexcept;
};

class UXX_EXPORT window {
    friend class scene;

public:
    class UXX_EXPORT properties {
    public:
        explicit properties() noexcept;
        ~properties() noexcept = default;

        properties(const properties&) = default;
        properties(properties&&) noexcept = default;
        properties& operator=(const properties&) = default;
        properties& operator=(properties&&) noexcept = default;

        [[nodiscard]] constexpr explicit operator int() const noexcept;

        properties clear() noexcept;
        properties set_no_title_bar() noexcept;
        properties set_no_resize() noexcept;
        properties set_no_move() noexcept;
        properties set_no_scrollbar() noexcept;
        properties set_no_scroll_with_mouse() noexcept;
        properties set_no_collapse() noexcept;
        properties set_always_auto_resize() noexcept;
        properties set_no_background() noexcept;
        properties set_no_saved_settings() noexcept;
        properties set_no_mouse_inputs() noexcept;
        properties set_menu_bar() noexcept;
        properties set_horizontal_scrollbar() noexcept;
        properties set_no_focus_on_appearing() noexcept;
        properties set_no_bring_to_front_on_focus() noexcept;
        properties set_always_vertical_scrollbar() noexcept;
        properties set_always_horizontal_scrollbar() noexcept;
        properties set_always_use_window_padding() noexcept;
        properties set_no_nav_inputs() noexcept;
        properties set_no_nav_focus() noexcept;
        properties set_unsaved_document() noexcept;
        properties set_no_nav() noexcept;
        properties set_no_decoration() noexcept;
        properties set_no_inputs() noexcept;

    private:
        int _flags;
    };

    window(const window&) = delete;
    window(window&&) noexcept = default;
    window& operator=(const window&) = delete;
    window& operator=(window&&) noexcept = default;

    [[nodiscard]] vec2d get_position() const;
    [[nodiscard]] vec2d get_size() const;
    [[nodiscard]] vec2d get_cursor_screen_position() const; // TODO: Will probably be deprecated in ImGui
    [[nodiscard]] vec2d get_available_content_region() const; // TODO: Will probably be deprecated in ImGui
    [[nodiscard]] mouse get_mouse() const;

    [[nodiscard]] bool is_collapsed() const noexcept;
    [[nodiscard]] bool is_item_hovered() const;
    [[nodiscard]] bool is_item_active() const;

    // TODO: Rename to get_pencil?
    [[nodiscard]] pencil create_pencil() const noexcept;
    [[nodiscard]] pencil create_pencil(pencil::type type) const noexcept;

    void label(string_ref text) const;
    bool button(string_ref text) const;
    void same_line() const;
    void input_text(string_ref label, std::string& value) const;
    void check_box(string_ref label, bool& value) const;

    // TODO: Refactor this when I understand what it does
    void invisible_button(string_ref id, const vec2d& size);
    void open_popup_context_item(string_ref id) const;
    void push_item_width(float width) const;
    void pop_item_width() const;
    [[nodiscard]] float get_font_size() const;
    [[nodiscard]] float calc_item_width() const;
    [[nodiscard]] float get_frame_height() const;
    void drag_float(string_ref label, float& value, float v_speed, float v_min, float v_max, string_ref format) const;
    bool slider_int(string_ref label, int& value, int v_min, int v_max) const;
    void color_edit_4(string_ref label, float color[4]) const;
    void dummy(const vec2d& size) const;
    void same_line(float offset_from_start_x, float spacing_w) const;

    // TODO: Part of imgui style (needs refactoring)
    [[nodiscard]] vec2d get_item_inner_spacing() const;

    template <typename F, typename... Args>
    void tab_bar(string_ref id, F&& f, Args&&... args) requires function<F, uxx::tab_bar&, Args...>
    {
        // TODO: Consider if user wants to be called even when tab bar is not visible
        if (begin_tab_bar(id) == tab_bar::visible::yes) {
            uxx::tab_bar tab_bar(*this);
            f(tab_bar, std::forward<Args>(args)...);
            end_tab_bar();
        }
    }

    template <typename F, typename... Args>
    void popup(string_ref id, F&& f, Args&&... args) requires function<F, uxx::popup&, Args...>
    {
        // TODO: Consider if user wants to be called even when tab bar is not visible
        if (begin_popup(id) == popup::visible::yes) {
            uxx::popup pu {};
            f(pu, std::forward<Args>(args)...);
            end_popup();
        }
    }

    template <typename F, typename... Args>
    void clip_rect(const vec2d& min, const vec2d& max, const bool intersect_with_current_clip_rect, F&& f, Args&&... args) requires function<F, uxx::window&, Args...>
    {
        push_clip_rect(min, max, intersect_with_current_clip_rect);
        f(*this, std::forward<Args>(args)...);
        pop_clip_rect();
    }

private:
    enum class collapsed {
        yes,
        no
    };

    collapsed _collapsed;

    explicit window(collapsed collapsed) noexcept;
    ~window() noexcept = default;

    [[nodiscard]] tab_bar::visible begin_tab_bar(string_ref id) const;
    void end_tab_bar() const;

    [[nodiscard]] popup::visible begin_popup(string_ref id) const;
    void end_popup() const;

    void push_clip_rect(const vec2d& min, const vec2d& max, const bool intersect_with_current_clip_rect) const;
    void pop_clip_rect() const;
};

class UXX_EXPORT scene {
    friend class app;

public:
    scene(const scene&) = delete;
    scene(scene&&) noexcept = default;
    scene& operator=(const scene&) = delete;
    scene& operator=(scene&&) noexcept = default;

    template <typename F, typename... Args>
    void window(string_ref title, F&& f, Args&&... args) const requires function<F, uxx::window&, Args...>
    {
        window_impl(title, std::nullopt, window::properties {}, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void window(string_ref title, bool& open, F&& f, Args&&... args) const requires function<F, uxx::window&, Args...>
    {
        window_impl(title, open, window::properties {}, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void window(string_ref title, bool& open, const window::properties properties, F&& f, Args&&... args) const requires function<F, uxx::window&, Args...>
    {
        window_impl(title, open, properties, std::forward<F>(f), std::forward<Args>(args)...);
    }

private:
    explicit scene() noexcept = default;
    ~scene() noexcept = default;

    template <typename F, typename... Args>
    void window_impl(string_ref title, std::optional<std::reference_wrapper<bool>> open, const window::properties properties, F&& f, Args&&... args) const requires function<F, uxx::window&, Args...>
    {
        if (!open.has_value() || (open.has_value() && open->get())) {
            const auto collapsed = begin_window(title, open, properties);
            {
                uxx::window w { collapsed };
                f(w, std::forward<Args>(args)...);
            }
            end_window();
        }
    }

    [[nodiscard]] window::collapsed begin_window(string_ref title, std::optional<std::reference_wrapper<bool>> open, window::properties properties) const;
    void end_window() const;
};

class UXX_EXPORT app {
public:
    enum class exit_code : int { success = 0 };

    explicit app() noexcept = default;
    ~app() noexcept = default;

    app(const app&) = delete;
    app(app&&) noexcept = default;
    app& operator=(const app&) = delete;
    app& operator=(app&&) noexcept = default;

    template <typename F, typename... Args>
    [[nodiscard]] int run(F f, Args&&... args) const requires function<F, scene&, Args...>
    {
        scene c;
        mainloop([&]() {
            f(c, std::forward<Args>(args)...);
        });
        return static_cast<int>(_exit_code);
    }

private:
    exit_code _exit_code { exit_code::success };
    void mainloop(const std::function<void()>& render) const;
};

}

#endif