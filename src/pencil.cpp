#include "common.hpp"
#include "uxx/uxx.hpp"

namespace {

[[nodiscard]] ImDrawList& as_draw_list(const std::any& draw_list)
{
    return *std::any_cast<ImDrawList*>(draw_list);
}

}

unsigned int uxx::rgb_color::to_color_u32() const noexcept
{
    return ImGui::GetColorU32(ImVec4 { r, g, b, 1.0f });
}

unsigned int uxx::rgba_color::to_color_u32() const noexcept
{
    return ImGui::GetColorU32(ImVec4 { r, g, b, a });
}

uxx::pencil::pencil() noexcept
    : _draw_list(ImGui::GetWindowDrawList())
    , _color(ImGui::GetColorU32({ 0, 0, 0, 1.0f }))
    , _thickness(1.0f)
    , _rounding(0.0f)
{
}

void uxx::pencil::set_color(const uxx::rgb_color& color) noexcept
{
    _color = color.to_color_u32();
}

void uxx::pencil::set_color(const uxx::rgba_color& color) noexcept
{
    _color = color.to_color_u32();
}

void uxx::pencil::draw_line(const uxx::vec2d& from, const uxx::vec2d& to) const
{
    as_draw_list(_draw_list).AddLine({ from.x, from.y }, { to.x, to.y }, _color, _thickness);
}
