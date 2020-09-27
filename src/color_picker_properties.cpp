#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::color_picker_properties::color_picker_properties() noexcept
    : _flags(ImGuiColorEditFlags_None)
{
}

constexpr uxx::color_picker_properties::operator int() const noexcept
{
    return _flags;
}

uxx::color_picker_properties uxx::color_picker_properties::clear() noexcept
{
    _flags = ImGuiColorEditFlags_None;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_alpha() noexcept
{
    _flags |= ImGuiColorEditFlags_NoAlpha;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_picker() noexcept
{
    _flags |= ImGuiColorEditFlags_NoPicker;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_options() noexcept
{
    _flags |= ImGuiColorEditFlags_NoOptions;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_small_preview() noexcept
{
    _flags |= ImGuiColorEditFlags_NoSmallPreview;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_inputs() noexcept
{
    _flags |= ImGuiColorEditFlags_NoInputs;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_tooltip() noexcept
{
    _flags |= ImGuiColorEditFlags_NoTooltip;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_label() noexcept
{
    _flags |= ImGuiColorEditFlags_NoLabel;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_side_preview() noexcept
{
    _flags |= ImGuiColorEditFlags_NoSidePreview;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_drag_drop() noexcept
{
    _flags |= ImGuiColorEditFlags_NoDragDrop;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_no_border() noexcept
{
    _flags |= ImGuiColorEditFlags_NoBorder;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_alpha_bar() noexcept
{
    _flags |= ImGuiColorEditFlags_AlphaBar;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_alpha_preview() noexcept
{
    _flags |= ImGuiColorEditFlags_AlphaPreview;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_alpha_preview_half() noexcept
{
    _flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_hdr() noexcept
{
    _flags |= ImGuiColorEditFlags_HDR;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_display_rgb() noexcept
{
    _flags |= ImGuiColorEditFlags_RGB;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_display_hsv() noexcept
{
    _flags |= ImGuiColorEditFlags_HSV;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_display_hex() noexcept
{
    _flags |= ImGuiColorEditFlags_DisplayHex;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_uint8() noexcept
{
    _flags |= ImGuiColorEditFlags_Uint8;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_float() noexcept
{
    _flags |= ImGuiColorEditFlags_Float;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_picker_hue_bar() noexcept
{
    _flags |= ImGuiColorEditFlags_PickerHueBar;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_picker_hue_wheel() noexcept
{
    _flags |= ImGuiColorEditFlags_PickerHueWheel;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_input_rgb() noexcept
{
    _flags |= ImGuiColorEditFlags_InputRGB;
    return *this;
}

uxx::color_picker_properties uxx::color_picker_properties::set_input_hsv() noexcept
{
    _flags |= ImGuiColorEditFlags_InputHSV;
    return *this;
}
