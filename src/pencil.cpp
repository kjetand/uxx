#include "common.hpp"
#include "uxx/uxx.hpp"

#ifdef _WIN32
#include <algorithm>
#else
#include <ranges>
#endif

namespace {

[[nodiscard]] ImDrawList& cast_draw_list(const std::any& draw_list)
{
    return *std::any_cast<ImDrawList*>(draw_list);
}

[[nodiscard]] ImVec2 from_vec2d(const uxx::vec2d& point) noexcept
{
    return ImVec2 { point.x, point.y };
}

[[nodiscard]] std::vector<ImVec2> from_vec2d(const std::vector<uxx::vec2d>& points) noexcept
{
    static const auto convert = [](const uxx::vec2d& p) { return from_vec2d(p); };
    std::vector<ImVec2> copy;
#ifdef _WIN32
    std::transform(points.cbegin(), points.cend(), std::back_inserter(copy), convert);
#else
    std::ranges::copy(std::ranges::views::transform(points, convert), std::back_inserter(copy));
#endif
    return copy;
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

uxx::pencil::properties::properties() noexcept
    : _flags(ImDrawListFlags_None)
{
}

uxx::pencil::properties::operator int() const noexcept
{
    return _flags;
}
uxx::pencil::properties uxx::pencil::properties::clear() noexcept
{
    _flags |= ImDrawListFlags_None;
    return *this;
}
uxx::pencil::properties uxx::pencil::properties::set_anti_aliased_lines() noexcept
{
    _flags |= ImDrawListFlags_AntiAliasedLines;
    return *this;
}
uxx::pencil::properties uxx::pencil::properties::set_anti_aliased_lines_using_textures() noexcept
{
    _flags |= ImDrawListFlags_AntiAliasedLinesUseTex;
    return *this;
}
uxx::pencil::properties uxx::pencil::properties::set_anti_aliased_fill() noexcept
{
    _flags |= ImDrawListFlags_AntiAliasedFill;
    return *this;
}

uxx::pencil::corner_properties::corner_properties() noexcept
    : _flags(ImDrawCornerFlags_None)
{
}

uxx::pencil::corner_properties::operator int() const noexcept
{
    return _flags;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::clear() noexcept
{
    _flags = ImDrawCornerFlags_None;
    return *this;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::set_top_left() noexcept
{
    _flags |= ImDrawCornerFlags_TopLeft;
    return *this;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::set_top_right() noexcept
{
    _flags |= ImDrawCornerFlags_TopRight;
    return *this;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::set_bottom_left() noexcept
{
    _flags |= ImDrawCornerFlags_BotLeft;
    return *this;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::set_bottom_right() noexcept
{
    _flags |= ImDrawCornerFlags_BotRight;
    return *this;
}
uxx::pencil::corner_properties uxx::pencil::corner_properties::set_all() noexcept
{
    _flags = ImDrawCornerFlags_All;
    return *this;
}

uxx::pencil::pencil() noexcept
    : _draw_list(ImGui::GetWindowDrawList())
    , _color(ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }))
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

void uxx::pencil::set_properties(const uxx::pencil::properties& props) noexcept
{
    cast_draw_list(_draw_list).Flags = static_cast<int>(props);
}

void uxx::pencil::set_corner_properties(const corner_properties& corner_props) noexcept
{
    _corner_props = corner_props;
}

void uxx::pencil::set_thickness(float thickness) noexcept
{
    _thickness = thickness;
}

void uxx::pencil::set_rounding(float rounding) noexcept
{
    _rounding = rounding;
}

void uxx::pencil::draw_line(const uxx::vec2d& from, const uxx::vec2d& to) const
{
    cast_draw_list(_draw_list).AddLine(from_vec2d(from), from_vec2d(to), _color, _thickness);
}

void uxx::pencil::draw_rect(const uxx::vec2d& min, const uxx::vec2d& max) const
{
    cast_draw_list(_draw_list).AddRect(from_vec2d(min), from_vec2d(max), _color, _rounding, static_cast<ImDrawCornerFlags>(_corner_props), _thickness);
}

void uxx::pencil::draw_rect_filled(const uxx::vec2d& min, const uxx::vec2d& max) const
{
    cast_draw_list(_draw_list).AddRectFilled(from_vec2d(min), from_vec2d(max), _color, _rounding, static_cast<ImDrawCornerFlags>(_corner_props));
}

void uxx::pencil::draw_rect_filled_multi_color(const uxx::vec2d& min, const uxx::vec2d& max, const uxx::color_rect& colors) const
{
    cast_draw_list(_draw_list).AddRectFilledMultiColor(from_vec2d(min), from_vec2d(max), colors.upper_left.to_color_u32(), colors.upper_right.to_color_u32(), colors.bottom_right.to_color_u32(), colors.bottom_left.to_color_u32());
}

void uxx::pencil::draw_quad(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3, const uxx::vec2d& p4) const
{
    cast_draw_list(_draw_list).AddQuad(from_vec2d(p1), from_vec2d(p2), from_vec2d(p3), from_vec2d(p4), _color, _thickness);
}

void uxx::pencil::draw_quad_filled(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3, const uxx::vec2d& p4) const
{
    cast_draw_list(_draw_list).AddQuadFilled(from_vec2d(p1), from_vec2d(p2), from_vec2d(p3), from_vec2d(p4), _color);
}

void uxx::pencil::draw_triangle(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3) const
{
    cast_draw_list(_draw_list).AddTriangle(from_vec2d(p1), from_vec2d(p2), from_vec2d(p3), _color, _thickness);
}

void uxx::pencil::draw_triangle_filled(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3) const
{
    cast_draw_list(_draw_list).AddTriangleFilled(from_vec2d(p1), from_vec2d(p2), from_vec2d(p3), _color);
}

void uxx::pencil::draw_circle(const uxx::vec2d& center, float radius) const
{
    draw_circle(center, radius, 0);
}

void uxx::pencil::draw_circle(const uxx::vec2d& center, float radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddCircle(from_vec2d(center), radius, _color, num_segments, _thickness);
}

void uxx::pencil::draw_circle_filled(const uxx::vec2d& center, float radius) const
{
    draw_circle_filled(center, radius, 0);
}

void uxx::pencil::draw_circle_filled(const uxx::vec2d& center, float radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddCircleFilled(from_vec2d(center), radius, _color, num_segments);
}

void uxx::pencil::draw_ngon(const uxx::vec2d& center, float radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddNgon(from_vec2d(center), radius, _color, num_segments, _thickness);
}

void uxx::pencil::draw_ngon_filled(const uxx::vec2d& center, float radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddNgonFilled(from_vec2d(center), radius, _color, num_segments);
}

void uxx::pencil::draw_polyline(const std::vector<uxx::vec2d>& points, bool closed) const
{
    const auto copy = from_vec2d(points);
    cast_draw_list(_draw_list).AddPolyline(copy.data(), static_cast<int>(copy.size()), _color, closed, _thickness);
}

void uxx::pencil::draw_convex_poly_filled(const std::vector<uxx::vec2d>& points) const
{
    const auto copy = from_vec2d(points);
    cast_draw_list(_draw_list).AddConvexPolyFilled(copy.data(), static_cast<int>(copy.size()), _color);
}

void uxx::pencil::draw_bezier_curve(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3, const uxx::vec2d& p4) const
{
    draw_bezier_curve(p1, p2, p3, p4, 0);
}

void uxx::pencil::draw_bezier_curve(const uxx::vec2d& p1, const uxx::vec2d& p2, const uxx::vec2d& p3, const uxx::vec2d& p4, int num_segments) const
{
    cast_draw_list(_draw_list).AddBezierCurve(from_vec2d(p1), from_vec2d(p2), from_vec2d(p3), from_vec2d(p4), _color, _thickness, num_segments);
}
