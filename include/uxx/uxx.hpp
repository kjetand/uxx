#ifndef _UXX_HPP
#define _UXX_HPP

#include <concepts>
#include <functional>
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
    explicit window() noexcept = default;
    ~window() noexcept = default;

    window(const window&) = delete;
    window(window&&) noexcept = default;
    window& operator=(const window&) = delete;
    window& operator=(window&&) noexcept = default;
};

class UXX_EXPORT canvas {
public:
    explicit canvas() noexcept = default;
    ~canvas() noexcept = default;

    canvas(const canvas&) = delete;
    canvas(canvas&&) noexcept = default;
    canvas& operator=(const canvas&) = delete;
    canvas& operator=(canvas&&) noexcept = default;

    template <typename F, typename... Args>
    void window(string_ref title, F f, Args&&... args) const requires function<F, window&, Args...>
    {
        begin_window(title);
        {
            uxx::window w {};
            f(w, std::forward<Args>(args)...);
        }
        end_window();
    }

private:
    void begin_window(string_ref title) const;
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
    [[nodiscard]] int run(F f, Args&&... args) const requires function<F, canvas&, Args...>
    {
        canvas c;
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