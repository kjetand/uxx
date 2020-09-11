#include "uxx/uxx.hpp"

#ifdef _WIN32
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#pragma GCC diagnostic pop
#endif

uxx::string_ref::string_ref(const std::string& str) noexcept
    : _str(str.c_str())
{
}

void uxx::app::mainloop(const std::function<void()>& render) const
{
    sf::RenderWindow w(sf::VideoMode(800, 600), "SFML window");
    w.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(w);

    sf::Event event {};
    sf::Clock delta_clock {};

    while (w.isOpen()) {
        while (w.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                w.close();
            } else {
                ImGui::SFML::ProcessEvent(event);
            }
        }
        ImGui::SFML::Update(w, delta_clock.restart());
        w.clear();
        render();
        ImGui::SFML::Render(w);

        w.display();
        sf::sleep(sf::milliseconds(10));
    }
    ImGui::SFML::Shutdown();
}

uxx::window::collapsed uxx::scene::begin_window(uxx::string_ref title, std::optional<std::reference_wrapper<bool>> open, uxx::window::properties properties) const
{
    if (open) {
        return ImGui::Begin(title, &open->get(), static_cast<int>(properties)) ? window::collapsed::no : window::collapsed::yes;
    }
    return ImGui::Begin(title) ? window::collapsed::no : window::collapsed::yes;
}

void uxx::scene::end_window() const
{
    ImGui::End();
}

constexpr uxx::window::properties::operator int() const noexcept
{
    return static_cast<int>(_flags);
}

uxx::window::properties uxx::window::properties::clear() noexcept
{
    _flags = ImGuiWindowFlags_None;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_title_bar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoTitleBar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_resize() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoResize);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_move() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoMove);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_scrollbar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoScrollbar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_scroll_with_mouse() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoScrollWithMouse);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_collapse() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoCollapse);
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_auto_resize() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_AlwaysAutoResize);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_background() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoBackground);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_saved_settings() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoSavedSettings);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_mouse_inputs() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoMouseInputs);
    return *this;
}

uxx::window::properties uxx::window::properties::set_menu_bar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_MenuBar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_horizontal_scrollbar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_HorizontalScrollbar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_focus_on_appearing() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoFocusOnAppearing);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_bring_to_front_on_focus() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoBringToFrontOnFocus);
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_vertical_scrollbar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_AlwaysVerticalScrollbar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_horizontal_scrollbar() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_use_window_padding() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_AlwaysUseWindowPadding);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav_inputs() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoNavInputs);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav_focus() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoNavFocus);
    return *this;
}

uxx::window::properties uxx::window::properties::set_unsaved_document() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_UnsavedDocument);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoNav);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_decoration() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoDecoration);
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_inputs() noexcept
{
    _flags |= static_cast<unsigned int>(ImGuiWindowFlags_NoInputs);
    return *this;
}

uxx::window::window(uxx::window::collapsed collapse) noexcept
    : _collapsed(collapse)
{
}

bool uxx::window::is_collapsed() const noexcept
{
    return _collapsed == collapsed::yes;
}

void uxx::window::label(uxx::string_ref text) const
{
    ImGui::Text("%s", text.c_str());
}

bool uxx::window::button(uxx::string_ref text) const
{
    return ImGui::Button(text);
}

void uxx::window::same_line() const
{
    ImGui::SameLine();
}

void uxx::window::input_text(uxx::string_ref label, std::string& value) const
{
    static const auto string_appender = [](ImGuiInputTextCallbackData* data) {
        auto* user_data = reinterpret_cast<std::string*>(data->UserData);
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            IM_ASSERT(data->Buf == user_data->c_str());
            user_data->resize(static_cast<std::size_t>(data->BufTextLen));
            data->Buf = const_cast<char*>(user_data->c_str());
        }
        return 0;
    };
    const ImGuiInputTextFlags flags = ImGuiInputTextFlags_None | ImGuiInputTextFlags_CallbackResize;
    auto* raw_value = const_cast<char*>(value.c_str());
    const auto buf_size = value.capacity() + 1;

    ImGui::InputText(label, raw_value, buf_size, flags, string_appender, &value);
}
