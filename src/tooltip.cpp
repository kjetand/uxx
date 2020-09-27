#include "common.hpp"
#include "uxx/uxx.hpp"

static auto get_tooltip_position()
{
    const auto position = ImGui::GetCursorScreenPos();
    return uxx::vec2d { position.x, position.y };
}

uxx::tooltip::tooltip()
    : _position(get_tooltip_position())
{
}

uxx::pencil uxx::tooltip::create_pencil() const
{
    return pencil();
}

uxx::vec2d uxx::tooltip::get_position() const noexcept
{
    return _position;
}

void uxx::tooltip::empty_space(const uxx::vec2d& size) const
{
    ImGui::Dummy({ size.x, size.y });
}
