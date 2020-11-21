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
    std::ranges::transform(points, std::back_inserter(copy), convert);
#endif
    return copy;
}

}

uxx::pencil::corner_properties::corner_properties() noexcept
    : _flags(ImDrawCornerFlags_None)
{
    static_assert(std::is_same_v<decltype(_flags), ImDrawCornerFlags>);
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
    : pencil(type::window)
{
}

static auto get_draw_list(const uxx::pencil::type type) noexcept
{
    switch (type) {
    case uxx::pencil::background:
        return ImGui::GetBackgroundDrawList();
    case uxx::pencil::foreground:
        return ImGui::GetForegroundDrawList();
    case uxx::pencil::window:
    default:
        return ImGui::GetWindowDrawList();
    }
}

uxx::pencil::pencil(const uxx::pencil::type pencil_type) noexcept
    : _draw_list(get_draw_list(pencil_type))
    , _color(ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }))
    , _thickness(1.0f)
    , _rounding(0.0f)
{
    cast_draw_list(_draw_list).Flags = ImDrawListFlags_None | ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;
}

void uxx::pencil::set_color(const uxx::rgb_color& color) noexcept
{
    _color = color.to_color32();
}

void uxx::pencil::set_color(const uxx::rgba_color& color) noexcept
{
    _color = color.to_color32();
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
    cast_draw_list(_draw_list).AddRectFilledMultiColor(from_vec2d(min), from_vec2d(max), colors.upper_left.to_color32(), colors.upper_right.to_color32(), colors.bottom_right.to_color32(), colors.bottom_left.to_color32());
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

void uxx::pencil::draw_circle(const uxx::vec2d& center, const uxx::radius radius) const
{
    draw_circle(center, radius, 0);
}

void uxx::pencil::draw_circle(const uxx::vec2d& center, const uxx::radius radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddCircle(from_vec2d(center), radius.get(), _color, num_segments, _thickness);
}

void uxx::pencil::draw_circle_filled(const uxx::vec2d& center, const uxx::radius radius) const
{
    draw_circle_filled(center, radius, 0);
}

void uxx::pencil::draw_circle_filled(const uxx::vec2d& center, const uxx::radius radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddCircleFilled(from_vec2d(center), radius.get(), _color, num_segments);
}

void uxx::pencil::draw_ngon(const uxx::vec2d& center, const uxx::radius radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddNgon(from_vec2d(center), radius.get(), _color, num_segments, _thickness);
}

void uxx::pencil::draw_ngon_filled(const uxx::vec2d& center, const uxx::radius radius, int num_segments) const
{
    cast_draw_list(_draw_list).AddNgonFilled(from_vec2d(center), radius.get(), _color, num_segments);
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

void uxx::pencil::push_clip_rect(const uxx::vec2d& min, const uxx::vec2d& max, const bool intersect_with_current_clip_rect) const
{
    ImGui::PushClipRect({ min.x, min.y }, { max.x, max.y }, intersect_with_current_clip_rect);
}

void uxx::pencil::pop_clip_rect() const
{
    ImGui::PopClipRect();
}

void uxx::pencil::draw_image(const uxx::image& image) const
{
    if (image._texture) {
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(image.get_native_handle())), { static_cast<float>(image.get_width()), static_cast<float>(image.get_height()) });
    }
}
