#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::window::collapsed uxx::scene::begin_window(uxx::string_ref title) const
{
    return ImGui::Begin(title) ? window::collapsed::no : window::collapsed::yes;
}

uxx::window::collapsed uxx::scene::begin_window(uxx::string_ref title, uxx::result<bool>& open, uxx::window::properties properties) const
{
    return ImGui::Begin(title, &open.get(), static_cast<int>(properties)) ? window::collapsed::no : window::collapsed::yes;
}

void uxx::scene::end_window() const
{
    ImGui::End();
}

uxx::window::properties::properties() noexcept
    : _flags(ImGuiWindowFlags_None)
{
    static_assert(std::is_same_v<decltype(_flags), ImGuiWindowFlags>);
}

constexpr uxx::window::properties::operator int() const noexcept
{
    return _flags;
}

uxx::window::properties uxx::window::properties::clear() noexcept
{
    _flags = ImGuiWindowFlags_None;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_title_bar() noexcept
{
    _flags |= ImGuiWindowFlags_NoTitleBar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_resize() noexcept
{
    _flags |= ImGuiWindowFlags_NoResize;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_move() noexcept
{
    _flags |= ImGuiWindowFlags_NoMove;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_scrollbar() noexcept
{
    _flags |= ImGuiWindowFlags_NoScrollbar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_scroll_with_mouse() noexcept
{
    _flags |= ImGuiWindowFlags_NoScrollWithMouse;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_collapse() noexcept
{
    _flags |= ImGuiWindowFlags_NoCollapse;
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_auto_resize() noexcept
{
    _flags |= ImGuiWindowFlags_AlwaysAutoResize;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_background() noexcept
{
    _flags |= ImGuiWindowFlags_NoBackground;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_saved_settings() noexcept
{
    _flags |= ImGuiWindowFlags_NoSavedSettings;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_mouse_inputs() noexcept
{
    _flags |= ImGuiWindowFlags_NoMouseInputs;
    return *this;
}

uxx::window::properties uxx::window::properties::set_menu_bar() noexcept
{
    _flags |= ImGuiWindowFlags_MenuBar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_horizontal_scrollbar() noexcept
{
    _flags |= ImGuiWindowFlags_HorizontalScrollbar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_focus_on_appearing() noexcept
{
    _flags |= ImGuiWindowFlags_NoFocusOnAppearing;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_bring_to_front_on_focus() noexcept
{
    _flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_vertical_scrollbar() noexcept
{
    _flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_horizontal_scrollbar() noexcept
{
    _flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    return *this;
}

uxx::window::properties uxx::window::properties::set_always_use_window_padding() noexcept
{
    _flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav_inputs() noexcept
{
    _flags |= ImGuiWindowFlags_NoNavInputs;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav_focus() noexcept
{
    _flags |= ImGuiWindowFlags_NoNavFocus;
    return *this;
}

uxx::window::properties uxx::window::properties::set_unsaved_document() noexcept
{
    _flags |= ImGuiWindowFlags_UnsavedDocument;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_nav() noexcept
{
    _flags |= ImGuiWindowFlags_NoNav;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_decoration() noexcept
{
    _flags |= ImGuiWindowFlags_NoDecoration;
    return *this;
}

uxx::window::properties uxx::window::properties::set_no_inputs() noexcept
{
    _flags |= ImGuiWindowFlags_NoInputs;
    return *this;
}

uxx::window::window(uxx::window::collapsed collapse) noexcept
    : _collapsed(collapse)
{
}

uxx::vec2d uxx::window::get_position() const
{
    const auto pos = ImGui::GetWindowPos();
    return vec2d { pos.x, pos.y };
}

uxx::vec2d uxx::window::get_size() const
{
    const auto size = ImGui::GetWindowSize();
    return vec2d { size.x, size.y };
}

uxx::vec2d uxx::window::get_cursor_screen_position() const
{
    const auto pos = ImGui::GetCursorScreenPos();
    return { pos.x, pos.y };
}

uxx::vec2d uxx::window::get_content_size() const
{
    const auto pos = ImGui::GetContentRegionAvail();
    return { pos.x, pos.y };
}

uxx::mouse uxx::window::get_mouse() const
{
    return mouse {};
}

uxx::pencil uxx::window::create_pencil() const noexcept
{
    return pencil();
}

uxx::pencil uxx::window::create_pencil_foreground() const noexcept
{
    return pencil(pencil::type::foreground);
}

uxx::pencil uxx::window::create_pencil_background() const noexcept
{
    return pencil(pencil::type::background);
}

bool uxx::window::is_collapsed() const noexcept
{
    return _collapsed == collapsed::yes;
}

bool uxx::window::is_item_hovered() const
{
    return ImGui::IsItemHovered();
}

bool uxx::window::is_item_active() const
{
    return ImGui::IsItemActive();
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

void uxx::window::input_text(uxx::string_ref label, uxx::result<std::string>& value) const
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
    auto* raw_value = const_cast<char*>(value.get().c_str());
    const auto buf_size = value.get().capacity() + 1;

    ImGui::InputText(label, raw_value, buf_size, flags, string_appender, &value);
}

bool uxx::window::checkbox(uxx::string_ref label, result<bool>& value) const
{
    return ImGui::Checkbox(label, &value.get());
}

bool uxx::window::slider_float(uxx::string_ref label, uxx::result<float>& value, const uxx::min<float> value_min, const uxx::max<float> value_max) const
{
    return ImGui::SliderFloat(label, &value.get(), value_min, value_max, "%.02f");
}

bool uxx::window::slider_float(uxx::string_ref label, uxx::multi_result<float, float>& value, uxx::min<float> value_min, uxx::max<float> value_max) const
{
    static float values[2];
    values[0] = std::get<0>(value);
    values[1] = std::get<1>(value);
    const bool changed = ImGui::SliderFloat2(label, values, value_min, value_max, "%.02f");
    std::get<0>(value) = values[0];
    std::get<1>(value) = values[1];
    return changed;
}

bool uxx::window::slider_int(uxx::string_ref label, uxx::result<int>& value, const uxx::min<int> value_min, const uxx::max<int> value_max) const
{
    return ImGui::SliderInt(label, &value.get(), value_min, value_max);
}

void uxx::window::drag_float(uxx::string_ref label, uxx::result<float>& value, drag_speed<float> speed, min<float> value_min, max<float> value_max) const
{
    ImGui::DragFloat(label, &value.get(), speed, value_min, value_max, "%.02f");
}

void uxx::window::color_picker(uxx::string_ref label, result<rgba_color>& color) const
{
    ImGui::ColorEdit4(label, &color.get().r);
}

void uxx::window::invisible_button(uxx::string_ref id, const uxx::vec2d& size) const
{
    // TODO: Don't hard code mouse click flags
    // TODO: Return "pressed"-bool?
    ImGui::InvisibleButton(id, ImVec2 { size.x, size.y }, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
}

void uxx::window::empty_space(const uxx::vec2d& size) const
{
    ImGui::Dummy({ size.x, size.y });
}

void uxx::window::separator() const
{
    ImGui::Separator();
}

void uxx::window::begin_tooltip() const
{
    ImGui::BeginTooltip();
}

void uxx::window::end_tooltip() const
{
    ImGui::EndTooltip();
}

void uxx::window::push_item_width(const float width) const
{
    ImGui::PushItemWidth(width);
}

void uxx::window::pop_item_width() const
{
    ImGui::PopItemWidth();
}
