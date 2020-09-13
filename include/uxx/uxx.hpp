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

struct rgb_color {
    float r;
    float g;
    float b;

    [[nodiscard]] unsigned int to_color_u32() const noexcept;
};

struct rgba_color {
    float r;
    float g;
    float b;
    float a;

    [[nodiscard]] unsigned int to_color_u32() const noexcept;
};

struct color_rect {
    rgba_color upper_left;
    rgba_color upper_right;
    rgba_color bottom_right;
    rgba_color& bottom_left;
};

class pencil {
    friend class window;

public:
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
};

class UXX_EXPORT window {
    friend class scene;

public:
    enum class collapsed {
        yes,
        no
    };

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

    [[nodiscard]] bool is_collapsed() const noexcept;

    [[nodiscard]] pencil create_pencil() const noexcept;

    void label(string_ref text) const;

    bool button(string_ref text) const;

    void same_line() const;

    void input_text(string_ref label, std::string& value) const;

private:
    collapsed _collapsed;

    explicit window(collapsed collapsed) noexcept;
    ~window() noexcept = default;
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
        if (open && open->get()) {
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