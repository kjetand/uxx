#include "common.hpp"
#include "uxx/uxx.hpp"

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
uxx::vec2d uxx::window::get_available_content_region() const
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
    return uxx::pencil();
}

uxx::pencil uxx::window::create_pencil(uxx::pencil::type type) const noexcept
{
    return uxx::pencil(type);
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

void uxx::window::check_box(uxx::string_ref label, bool& value) const
{
    ImGui::Checkbox(label, &value);
}

void uxx::window::invisible_button(uxx::string_ref id, const uxx::vec2d& size)
{
    // TODO: Don't hard code mouse click flags
    // TODO: Return "pressed"-bool?
    ImGui::InvisibleButton(id, ImVec2 { size.x, size.y }, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
}

void uxx::window::open_popup_context_item(uxx::string_ref id) const
{
    ImGui::OpenPopupContextItem(id);
}

uxx::popup::visible uxx::window::begin_popup(uxx::string_ref id) const
{
    return ImGui::BeginPopup(id) ? popup::visible::yes : popup::visible::no;
}

void uxx::window::end_popup() const
{
    ImGui::EndPopup();
}

void uxx::window::push_clip_rect(const uxx::vec2d& min, const uxx::vec2d& max, const bool intersect_with_current_clip_rect) const
{
    ImGui::PushClipRect({ min.x, min.y }, { max.x, max.y }, intersect_with_current_clip_rect);
}

void uxx::window::pop_clip_rect() const
{
    ImGui::PopClipRect();
}
void uxx::window::push_item_width(float width) const
{
    ImGui::PushItemWidth(width);
}

void uxx::window::pop_item_width() const
{
    ImGui::PopItemWidth();
}

float uxx::window::get_font_size() const
{
    return ImGui::GetFontSize();
}

float uxx::window::calc_item_width() const
{
    return ImGui::CalcItemWidth();
}

float uxx::window::get_frame_height() const
{
    return ImGui::GetFrameHeight();
}

void uxx::window::drag_float(uxx::string_ref label, float& value, float v_speed, float v_min, float v_max, uxx::string_ref format) const
{
    ImGui::DragFloat(label, &value, v_speed, v_min, v_max, format);
}

bool uxx::window::slider_int(uxx::string_ref label, int& value, int v_min, int v_max) const
{
    return ImGui::SliderInt(label, &value, v_min, v_max);
}

void uxx::window::color_edit_4(uxx::string_ref label, float* color) const
{
    ImGui::ColorEdit4(label, color);
}

void uxx::window::dummy(const uxx::vec2d& size) const
{
    ImGui::Dummy({ size.x, size.y });
}

void uxx::window::same_line(float offset_from_start_x, float spacing_w) const
{
    ImGui::SameLine(offset_from_start_x, spacing_w);
}
uxx::vec2d uxx::window::get_item_inner_spacing() const
{
    const auto inner_spacing = ImGui::GetStyle().ItemInnerSpacing;
    return { inner_spacing.x, inner_spacing.y };
}
