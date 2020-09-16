#include "test.hpp"
#include "uxx/uxx.hpp"

TEST_CASE("Converts float colors to integer color", "[color]") {
    UXX_SUPPRESS_EXTERNAL_WARNINGS({
        REQUIRE(uxx::color_float_to_uint8(-0.1f) == IM_F32_TO_INT8_SAT(-0.1f));
        REQUIRE(uxx::color_float_to_uint8(0.0f) == IM_F32_TO_INT8_SAT(0.0f));
        REQUIRE(uxx::color_float_to_uint8(0.2f) == IM_F32_TO_INT8_SAT(0.2f));
        REQUIRE(uxx::color_float_to_uint8(0.5f) == IM_F32_TO_INT8_SAT(0.5f));
        REQUIRE(uxx::color_float_to_uint8(0.7f) == IM_F32_TO_INT8_SAT(0.7f));
        REQUIRE(uxx::color_float_to_uint8(1.0f) == IM_F32_TO_INT8_SAT(1.0f));
        REQUIRE(uxx::color_float_to_uint8(1.1f) == IM_F32_TO_INT8_SAT(1.1f));
    })
}

TEST_CASE("Converts from integers to 32-bit color", "[color]")
{
    constexpr std::uint8_t r = 123, g = 87, b = 12, a = 100;
    constexpr auto uxx_color = uxx::rgba_color::to_color32(r, g, b, a);

    UXX_SUPPRESS_EXTERNAL_WARNINGS({
        REQUIRE(uxx_color == IM_COL32(r, g, b, a));
    })
}

TEST_CASE("Converts from integers to RGBA color", "[color]")
{
    constexpr std::uint8_t r = 255, g = 108, b = 32, a = 155;
    constexpr auto uxx_color = uxx::rgba_color::from_integers(r, g, b, a);

    UXX_SUPPRESS_EXTERNAL_WARNINGS({
        const auto imgui_color = ImGui::ColorConvertU32ToFloat4(IM_COL32(r, g, b, a));
        REQUIRE(uxx_color.r == imgui_color.x);
        REQUIRE(uxx_color.g == imgui_color.y);
        REQUIRE(uxx_color.b == imgui_color.z);
        REQUIRE(uxx_color.a == imgui_color.w);
    })
}
