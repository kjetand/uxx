#ifndef _UXX_HPP
#define _UXX_HPP

#include <concepts>
#include <functional>
#include <optional>
#include <string>
#include <type_traits>

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

class UXX_EXPORT window {
public:
    enum class collapsed {
        yes,
        no
    };

    class UXX_EXPORT properties {
    public:
        explicit properties() noexcept = default;
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
        unsigned int _flags { 0 };
    };

    explicit window(collapsed collapsed) noexcept;
    ~window() noexcept = default;

    window(const window&) = delete;
    window(window&&) noexcept = default;
    window& operator=(const window&) = delete;
    window& operator=(window&&) noexcept = default;

    [[nodiscard]] bool is_collapsed() const noexcept;

    void label(string_ref text) const;

    bool button(string_ref text) const;

    void same_line() const;

    void input_text(string_ref label, std::string& value) const;

private:
    collapsed _collapsed;
};

class UXX_EXPORT scene {
public:
    explicit scene() noexcept = default;
    ~scene() noexcept = default;

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