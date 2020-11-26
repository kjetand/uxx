#ifndef _UXX_HPP
#define _UXX_HPP

#include <any>
#include <concepts>
#include <filesystem>
#include <functional>
#include <memory>
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

class pane;

enum class type_property {
    copy_and_move,
    neither_copy_nor_move
};

template <typename T, typename Tag, type_property Property = type_property::copy_and_move>
class explicit_arg {
public:
    static constexpr bool is_copyable_and_movable = Property == type_property::copy_and_move;

    explicit explicit_arg() noexcept(std::is_nothrow_default_constructible_v<T>) requires std::is_default_constructible_v<T> { }
    explicit explicit_arg(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) requires std::is_copy_constructible_v<T> : _value(value) { }
    explicit explicit_arg(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) requires std::is_move_constructible_v<T> : _value(std::move(value)) { }

    ~explicit_arg() noexcept(std::is_nothrow_destructible_v<T>) = default;

    explicit_arg(const explicit_arg<T, Tag, Property>&) noexcept(std::is_nothrow_copy_constructible_v<T>) requires(std::is_copy_constructible_v<T>&& is_copyable_and_movable) = default;
    explicit_arg(explicit_arg<T, Tag, Property>&&) noexcept(std::is_nothrow_move_constructible_v<T>) requires(std::is_move_constructible_v<T>&& is_copyable_and_movable) = default;
    explicit_arg<T, Tag, Property>& operator=(const explicit_arg<T, Tag, Property>&) noexcept(std::is_nothrow_copy_assignable_v<T>) requires(std::is_copy_assignable_v<T>&& is_copyable_and_movable) = default;
    explicit_arg<T, Tag, Property>& operator=(explicit_arg<T, Tag, Property>&&) noexcept(std::is_nothrow_move_assignable_v<T>) requires(std::is_move_assignable_v<T>&& is_copyable_and_movable) = default;

    explicit_arg<T, Tag, Property>& operator=(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>) requires(std::is_copy_assignable_v<T> && not std::is_const_v<T>)
    {
        _value = value;
        return *this;
    }

    explicit_arg<T, Tag, Property>& operator=(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) requires(std::is_move_assignable_v<T> && not std::is_const_v<T>)
    {
        _value = std::move(value);
        return *this;
    }

    operator T() const noexcept requires(std::integral<T> || std::floating_point<T>)
    {
        return _value;
    }

    [[nodiscard]] T get() const noexcept(std::is_nothrow_copy_constructible_v<T>) requires(not std::is_reference_v<T>)
    {
        return _value;
    }

    [[nodiscard]] const std::remove_reference_t<T>& get() const noexcept
    {
        return _value;
    }

    [[nodiscard]] std::remove_reference_t<T>& get() noexcept requires(not std::is_const_v<T>)
    {
        return _value;
    }

    [[nodiscard]] bool operator==(const T& other) const requires std::equality_comparable<T>
    {
        return _value == other;
    }

    [[nodiscard]] bool operator==(const explicit_arg<T, Tag, Property>& other) const requires std::equality_comparable<T>
    {
        return _value == other._value;
    }

private:
    T _value;
};

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

[[nodiscard]] constexpr std::uint8_t color_float_to_uint8(const float f) noexcept
{
    const auto saturated_f = (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f
                                                            : f;
    return static_cast<std::uint8_t>(saturated_f * 255.0f + 0.5f);
}

struct UXX_EXPORT rgb_color {
    float r;
    float g;
    float b;

    [[nodiscard]] constexpr color32 to_color32() const noexcept
    {
        return static_cast<color32>((color_float_to_uint8(r) << 0) | (color_float_to_uint8(g) << 8) | (color_float_to_uint8(b) << 16) | (color_float_to_uint8(1.0f) << 24));
    }
};

struct UXX_EXPORT rgba_color {
    float r;
    float g;
    float b;
    float a;

    [[nodiscard]] constexpr color32 to_color32() const noexcept
    {
        return static_cast<color32>((color_float_to_uint8(r) << 0) | (color_float_to_uint8(g) << 8) | (color_float_to_uint8(b) << 16) | (color_float_to_uint8(a) << 24));
    }

    [[nodiscard]] static constexpr color32 to_color32(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
    {
        return static_cast<color32>((a << 24) | (b << 16) | (g << 8) | (r << 0));
    }

    [[nodiscard]] static constexpr rgba_color from_integers(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
    {
        const auto c32 = to_color32(r, g, b, a);
        constexpr float s = 1.0f / 255.0f;
        return rgba_color {
            static_cast<float>((c32 >> 0) & 0xFF) * s,
            static_cast<float>((c32 >> 8) & 0xFF) * s,
            static_cast<float>((c32 >> 16) & 0xFF) * s,
            static_cast<float>((c32 >> 24) & 0xFF) * s
        };
    }
};

struct color_rect {
    rgba_color upper_left;
    rgba_color upper_right;
    rgba_color bottom_right;
    rgba_color bottom_left;
};

namespace tags {
    struct radius {
    };
    struct min {
    };
    struct max {
    };
    struct out_value {
    };
    struct id {
    };
    struct width {
    };
    struct height {
    };
}

/// Explicit radius type
using radius = explicit_arg<float, tags::radius>;

/// Explicit minimum type
template <typename T>
using min = explicit_arg<T, tags::min>;

/// Explicit maximum type
template <typename T>
using max = explicit_arg<T, tags::max>;

/// Explicit out value type (neither copyable nor movable)
template <typename T>
using result = explicit_arg<T, tags::out_value, type_property::neither_copy_nor_move>;

/// Explicit identifier type
using id = explicit_arg<string_ref, tags::id>;

/// Explicit width type
using width = explicit_arg<float, tags::width>;

/// Explicit height type
using height = explicit_arg<float, tags::width>;

class image {
    friend class pane;

public:
    UXX_EXPORT explicit image(const std::filesystem::path& image_path) noexcept;
    UXX_EXPORT ~image() noexcept;

    image(const image&) = delete;
    image(image&&) noexcept = default;
    image& operator=(const image&) = delete;
    image& operator=(image&&) noexcept = default;

    [[nodiscard]] UXX_EXPORT float get_width() const noexcept;
    [[nodiscard]] UXX_EXPORT float get_height() const noexcept;

private:
    struct data;
    std::unique_ptr<data> _texture;

    [[nodiscard]] unsigned int get_native_handle() const;
};

class pencil {
    friend class pane;

public:
    enum type {
        window, // Default
        background,
        foreground
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
    UXX_EXPORT void set_corner_properties(const corner_properties& corner_props) noexcept;

    UXX_EXPORT void draw_line(const vec2d& from, const vec2d& to) const;
    UXX_EXPORT void draw_rect(const vec2d& min, const vec2d& max) const;
    UXX_EXPORT void draw_rect_filled(const vec2d& min, const vec2d& max) const;
    UXX_EXPORT void draw_rect_filled_multi_color(const vec2d& min, const vec2d& max, const color_rect& colors) const;
    UXX_EXPORT void draw_quad(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_quad_filled(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_triangle(const vec2d& p1, const vec2d& p2, const vec2d& p3) const;
    UXX_EXPORT void draw_triangle_filled(const vec2d& p1, const vec2d& p2, const vec2d& p3) const;
    UXX_EXPORT void draw_circle(const vec2d& center, uxx::radius radius) const;
    UXX_EXPORT void draw_circle(const vec2d& center, uxx::radius radius, int num_segments) const;
    UXX_EXPORT void draw_circle_filled(const vec2d& center, uxx::radius radius) const;
    UXX_EXPORT void draw_circle_filled(const vec2d& center, uxx::radius radius, int num_segments) const;
    UXX_EXPORT void draw_ngon(const vec2d& center, uxx::radius radius, int num_segments) const;
    UXX_EXPORT void draw_ngon_filled(const vec2d& center, uxx::radius radius, int num_segments) const;
    UXX_EXPORT void draw_polyline(const std::vector<vec2d>& points, bool closed) const;
    UXX_EXPORT void draw_convex_poly_filled(const std::vector<vec2d>& points) const;
    UXX_EXPORT void draw_bezier_curve(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4) const;
    UXX_EXPORT void draw_bezier_curve(const vec2d& p1, const vec2d& p2, const vec2d& p3, const vec2d& p4, int num_segments) const;

    // TODO: Draw text, images...

    template <typename F, typename... Args>
    void clip_rectangle(const vec2d& min, const vec2d& max, F&& f, Args&&... args) requires function<F, uxx::pencil&, Args...>
    {
        push_clip_rect(min, max, false);
        f(*this, std::forward<Args>(args)...);
        pop_clip_rect();
    }

private:
    std::any _draw_list;
    unsigned int _color;
    float _thickness;
    float _rounding;
    corner_properties _corner_props;

    explicit pencil() noexcept;
    explicit pencil(type pencil_type) noexcept;

    UXX_EXPORT void push_clip_rect(const vec2d& min, const vec2d& max, const bool intersect_with_current_clip_rect) const;
    UXX_EXPORT void pop_clip_rect() const;
};

class UXX_EXPORT tab_bar {
    friend class pane;

public:
    tab_bar(const tab_bar&) = delete;
    tab_bar(tab_bar&&) noexcept = default;
    tab_bar& operator=(const tab_bar&) = delete;
    tab_bar& operator=(tab_bar&&) noexcept = delete;

    // TODO: Implement tab bar/item properties

    template <typename F, typename... Args>
    void item(string_ref label, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
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

    uxx::pane& _window;

    explicit tab_bar(uxx::pane& w) noexcept;
    ~tab_bar() noexcept = default;

    [[nodiscard]] bool begin_tab_item(string_ref label) const;
    void end_tab_item() const;
};

class UXX_EXPORT popup {
    friend class canvas;

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
    friend class pane;
    friend class canvas;

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

class UXX_EXPORT canvas {
    friend class pane;

public:
    ~canvas() noexcept = default;

    canvas(const canvas&) = delete;
    canvas(canvas&&) noexcept = default;
    canvas& operator=(const canvas&) = delete;
    canvas& operator=(canvas&&) noexcept = default;

    /// \return Mouse interaction object for the current window.
    [[nodiscard]] mouse get_mouse() const;
    /// \return Position of the canvas relative to the application screen.
    [[nodiscard]] vec2d get_position() const;
    /// \return Size of the canvas.
    [[nodiscard]] vec2d get_size() const;
    /// \return True if canvas is hovered by mouse.
    [[nodiscard]] bool is_hovered() const;
    /// \return True if canvas is active by mouse interaction.
    [[nodiscard]] bool is_active() const;

    /// Show popup menu.
    /// \tparam F User provided callback type that is required to take a uxx::popup reference and optionally user provided argument types
    /// \tparam Args User provided argument types that will be required by 'F'
    /// \param id Identifier of this popup menu
    /// \param f User provided callback
    /// \param args Optional user provided arguments that are yielded to 'f'
    template <typename F, typename... Args>
    void popup(uxx::id id, F&& f, Args&&... args) requires function<F, uxx::popup&, Args...>
    {
        auto mouse = get_mouse();
        const auto drag_delta = mouse.get_drag_delta(uxx::mouse::button::right);

        if (mouse.is_released(uxx::mouse::button::right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
            open_popup_context_item(id);
        }
        if (begin_popup(id) == popup::visible::yes) {
            uxx::popup pu {};
            f(pu, std::forward<Args>(args)...);
            end_popup();
        }
    }

private:
    vec2d _position;
    vec2d _size;

    explicit canvas(const vec2d& position, const vec2d& size) noexcept;

    [[nodiscard]] popup::visible begin_popup(uxx::id id) const;
    void end_popup() const;
    void open_popup_context_item(uxx::id id) const;
};

class UXX_EXPORT pane {
    friend class screen;

public:
    class UXX_EXPORT properties {
    public:
        explicit properties() noexcept;
        ~properties() noexcept = default;

        properties(const properties&) = default;
        properties(properties&&) noexcept = default;
        properties& operator=(const properties&) = default;
        properties& operator=(properties&&) noexcept = default;

        [[nodiscard]] constexpr explicit operator int() const noexcept
        {
            return _flags;
        }

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

    pane(const pane&) = delete;
    pane(pane&&) noexcept = default;
    pane& operator=(const pane&) = delete;
    pane& operator=(pane&&) noexcept = default;

    /// \return Position of the current window relative to the application screen.
    [[nodiscard]] vec2d get_position() const;
    /// \return Size of entire window.
    [[nodiscard]] vec2d get_size() const;
    /// \return Size of entire content size of window (window size minus padding, borders etc.).
    [[nodiscard]] vec2d get_content_size() const;
    /// \return Position for the current cursor (where the next item will be placed) relative to the application screen.
    [[nodiscard]] vec2d get_cursor_screen_position() const;
    /// \return Mouse interaction object for the current window.
    [[nodiscard]] mouse get_mouse() const;

    /// \return True if current window is collapsed.
    [[nodiscard]] bool is_collapsed() const noexcept;
    /// \return True if the last item added is hovered by the mouse.
    [[nodiscard]] bool is_item_hovered() const;
    /// \return True if the last item added is active (e.g. pressed, edited, etc.).
    [[nodiscard]] bool is_item_active() const;

    /// \return Drawing API for the window canvas.
    [[nodiscard]] pencil create_pencil() const noexcept;
    /// \return Drawing API for the application foreground (drawn last for each frame).
    [[nodiscard]] pencil create_pencil_foreground() const noexcept;
    /// \return Drawing API for the application background (drawn first for each frame).
    [[nodiscard]] pencil create_pencil_background() const noexcept;

    /// Adds a text label to the current window.
    void label(string_ref text) const;
    /// Adds a clickable button to the current window.
    /// \return True if button is clicked.
    bool button(string_ref text) const;
    /// Adds an invisible area to the current window.
    void empty_space(const vec2d& size) const;
    /// Enforce that next item will be placed on the same line as the previous added item.
    void same_line() const;
    /// Add input text field to the current window.
    /// \param label A label added before the input text field.
    /// \param value Storage for the input text.
    void input_text(string_ref label, result<std::string>& value) const;
    /// Adds a checkbox to the current window.
    /// \param label A label added after the checkbox.
    /// \param value Storage for the current checked-value.
    void checkbox(string_ref label, result<bool>& value) const;
    /// Show color picker.
    /// \param label Text label for the color picker
    /// \param color Storage of the current picked color
    void color_picker(string_ref label, result<rgba_color>& color) const;
    /// Add slider (float) to the current window.
    /// \param label Text label added to the right of the slider.
    /// \param value Storage for current slider value.
    /// \param value_min Minimum allowed value.
    /// \param value_max Maximum allowed value.
    /// \return True if value is changed.
    bool slider_float(string_ref label, result<float>& value, min<float> value_min, max<float> value_max) const;
    /// Add slider (integer) to the current window.
    /// \param label Text label added to the right of the slider.
    /// \param value Storage for current slider value.
    /// \param value_min Minimum allowed value.
    /// \param value_max Maximum allowed value.
    /// \return True if value is changed.
    bool slider_int(string_ref label, result<int>& value, min<int> value_min, max<int> value_max) const;
    /// Draw image.
    /// \param image
    void draw_image(const uxx::image& image) const;
    /// Draw image width explicit width and height.
    /// \param image
    /// \param width
    /// \param height
    void draw_image(const uxx::image& image, const uxx::width width, const uxx::height height) const;

    template <typename F, typename... Args>
    void canvas(uxx::id id, const vec2d& size, F&& f, Args&&... args) requires function<F, uxx::canvas&, uxx::pencil&, Args...>
    {
        const auto position = get_cursor_screen_position();
        invisible_button(id, size);
        uxx::canvas c(position, size);
        auto pencil = create_pencil();
        f(c, pencil, std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void tab_bar(uxx::id id, F&& f, Args&&... args) requires function<F, uxx::tab_bar&, Args...>
    {
        if (begin_tab_bar(id) == tab_bar::visible::yes) {
            uxx::tab_bar tab_bar(*this);
            f(tab_bar, std::forward<Args>(args)...);
            end_tab_bar();
        }
    }

private:
    enum class collapsed {
        yes,
        no
    };

    collapsed _collapsed;

    explicit pane(collapsed collapsed) noexcept;
    ~pane() noexcept = default;

    [[nodiscard]] tab_bar::visible begin_tab_bar(uxx::id id) const;
    void end_tab_bar() const;
    void invisible_button(uxx::id id, const vec2d& size) const;
};

class UXX_EXPORT menu {
    friend class menu_bar;

public:
    ~menu() noexcept = default;

    menu(const menu&) = delete;
    menu(menu&&) noexcept = default;
    menu& operator=(const menu&) = delete;
    menu& operator=(menu&&) noexcept = default;

    bool item(string_ref label) const;
    void separator() const;

private:
    explicit menu() noexcept = default;
};

class UXX_EXPORT menu_bar {
    friend class screen;

public:
    ~menu_bar() noexcept = default;

    menu_bar(const menu_bar&) = delete;
    menu_bar(menu_bar&&) noexcept = default;
    menu_bar& operator=(const menu_bar&) = delete;
    menu_bar& operator=(menu_bar&&) noexcept = default;

    template <typename F, typename... Args>
    void menu(string_ref label, F&& f, Args&&... args) const requires function<F, uxx::menu&, Args...>
    {
        if (begin_menu(label)) {
            uxx::menu m;
            f(m, std::forward<Args>(args)...);
            end_menu();
        }
    }

private:
    explicit menu_bar() noexcept = default;

    [[nodiscard]] bool begin_menu(string_ref label) const;
    void end_menu() const;
};

class UXX_EXPORT screen {
    friend class app;

public:
    screen(const screen&) = delete;
    screen(screen&&) noexcept = default;
    screen& operator=(const screen&) = delete;
    screen& operator=(screen&&) noexcept = default;

    template <typename F, typename... Args>
    void window(string_ref title, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
    {
        window_impl(title, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void window(string_ref title, result<bool>& open, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
    {
        window_impl(title, open, pane::properties {}, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void window(string_ref title, result<bool>& open, const pane::properties properties, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
    {
        window_impl(title, open, properties, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    void menu_bar(F&& f, Args&&... args) const requires function<F, uxx::menu_bar&, Args...>
    {
        begin_main_menu_bar();
        uxx::menu_bar mb;
        f(mb, std::forward<Args>(args)...);
        end_main_menu_bar();
    }

private:
    explicit screen() noexcept = default;
    ~screen() noexcept = default;

    template <typename F, typename... Args>
    void window_impl(string_ref title, result<bool>& open, const pane::properties properties, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
    {
        if (open.get()) {
            const auto collapsed = begin_window(title, open, properties);
            {
                uxx::pane w { collapsed };
                f(w, std::forward<Args>(args)...);
            }
            end_window();
        }
    }

    template <typename F, typename... Args>
    void window_impl(string_ref title, F&& f, Args&&... args) const requires function<F, uxx::pane&, Args...>
    {
        const auto collapsed = begin_window(title);
        {
            uxx::pane w { collapsed };
            f(w, std::forward<Args>(args)...);
        }
        end_window();
    }

    [[nodiscard]] pane::collapsed begin_window(string_ref title) const;
    [[nodiscard]] pane::collapsed begin_window(string_ref title, result<bool>& open, pane::properties properties) const;
    void end_window() const;

    void begin_main_menu_bar() const;
    void end_main_menu_bar() const;
};

class UXX_EXPORT app {
public:
    static constexpr unsigned int DEFAULT_WIDTH { 800 };
    static constexpr unsigned int DEFAULT_HEIGHT { 600 };
    enum class exit_code : int { success = 0 };

    explicit app() noexcept = default;
    ~app() noexcept = default;

    app(const app&) = delete;
    app(app&&) noexcept = default;
    app& operator=(const app&) = delete;
    app& operator=(app&&) noexcept = default;

    void set_width(unsigned int width) noexcept;
    void set_height(unsigned int height) noexcept;

    template <typename F, typename... Args>
    [[nodiscard]] int run(string_ref title, F f, Args&&... args) const requires function<F, screen&, Args...>
    {
        screen c;
        mainloop(title, [&]() {
            f(c, std::forward<Args>(args)...);
        });
        return static_cast<int>(_exit_code);
    }

private:
    exit_code _exit_code { exit_code::success };
    unsigned int _width { uxx::app::DEFAULT_WIDTH };
    unsigned int _height { uxx::app::DEFAULT_HEIGHT };

    void mainloop(string_ref title, const std::function<void()>& render) const;
};
}

#endif