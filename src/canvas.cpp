#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::canvas::canvas(const vec2d& position, const vec2d& size) noexcept
    : _position(position)
    , _size(size)
{
}

uxx::vec2d uxx::canvas::get_position() const
{
    return _position;
}

uxx::vec2d uxx::canvas::get_size() const
{
    return _size;
}

uxx::mouse uxx::canvas::get_mouse() const
{
    return mouse {};
}

bool uxx::canvas::is_hovered() const
{
    return ImGui::IsItemHovered();
}
bool uxx::canvas::is_active() const
{
    return ImGui::IsItemActive();
}

void uxx::canvas::open_popup_context_item(uxx::string_ref id) const
{
    ImGui::OpenPopupContextItem(id);
}

uxx::popup::visible uxx::canvas::begin_popup(uxx::string_ref id) const
{
    return ImGui::BeginPopup(id) ? popup::visible::yes : popup::visible::no;
}

void uxx::canvas::end_popup() const
{
    ImGui::EndPopup();
}
