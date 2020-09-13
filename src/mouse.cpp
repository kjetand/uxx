#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::mouse::mouse() noexcept
    : _io(ImGui::GetIO())
{
}

float uxx::mouse::get_x() const
{
    return std::any_cast<const ImGuiIO&>(_io).MousePos.x;
}

float uxx::mouse::get_y() const
{
    return std::any_cast<const ImGuiIO&>(_io).MousePos.y;
}

float uxx::mouse::get_delta_x() const
{
    return std::any_cast<const ImGuiIO&>(_io).MouseDelta.x;
}

float uxx::mouse::get_delta_y() const
{
    return std::any_cast<const ImGuiIO&>(_io).MouseDelta.y;
}

static constexpr auto to_mouse_flag(const uxx::mouse::button button) noexcept
{
    switch (button) {
    case uxx::mouse::button::right:
        return ImGuiMouseButton_Right;
    case uxx::mouse::button::middle:
        return ImGuiMouseButton_Middle;
    case uxx::mouse::button::left:
    default:
        return ImGuiMouseButton_Left;
    }
}

uxx::vec2d uxx::mouse::get_drag_delta(const uxx::mouse::button b) const
{
    const auto drag_delta = ImGui::GetMouseDragDelta(to_mouse_flag(b));
    return { drag_delta.x, drag_delta.y };
}

bool uxx::mouse::is_clicked(const uxx::mouse::button b) const noexcept
{
    return ImGui::IsMouseClicked(to_mouse_flag(b));
}

bool uxx::mouse::is_down(const uxx::mouse::button b) const noexcept
{
    return ImGui::IsMouseDown(to_mouse_flag(b));
}

bool uxx::mouse::is_released(const uxx::mouse::button b) const noexcept
{
    return ImGui::IsMouseReleased(to_mouse_flag(b));
}

bool uxx::mouse::is_dragging(uxx::mouse::button b, float lock_threshold) const noexcept
{
    return ImGui::IsMouseDragging(to_mouse_flag(b), lock_threshold);
}
