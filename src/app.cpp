#include "common.hpp"
#include "uxx/uxx.hpp"

void uxx::app::mainloop(const std::function<void()>& render) const
{
    sf::RenderWindow w(sf::VideoMode(800, 600), "SFML window");
    w.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(w);

    sf::Event event {};
    sf::Clock delta_clock {};

    while (w.isOpen()) {
        while (w.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                w.close();
            } else {
                ImGui::SFML::ProcessEvent(event);
            }
        }
        ImGui::SFML::Update(w, delta_clock.restart());
        w.clear();
        render();
        ImGui::SFML::Render(w);

        w.display();
        sf::sleep(sf::milliseconds(10));
    }
    ImGui::SFML::Shutdown();
}

uxx::style& uxx::app::get_style()
{
    auto& imgui_style = ImGui::GetStyle();
    static uxx::style style {
        .Alpha = result<float> { imgui_style.Alpha },
        .WindowPadding = std::make_tuple(result<float> { imgui_style.WindowPadding.x }, result<float> { imgui_style.WindowPadding.y }),
        .WindowRounding = result<float>(imgui_style.WindowRounding),
        .WindowBorderSize = result<float>(imgui_style.WindowBorderSize),
        .WindowMinSize = std::make_tuple(result<float>(imgui_style.WindowMinSize.x), result<float>(imgui_style.WindowMinSize.y)),
        .WindowTitleAlign = std::make_tuple(result<float>(imgui_style.WindowTitleAlign.x), result<float>(imgui_style.WindowTitleAlign.y)),
        .WindowMenuButtonPosition = {},
        .ChildRounding = result<float>(imgui_style.ChildRounding),
        .ChildBorderSize = result<float>(imgui_style.ChildBorderSize),
        .PopupRounding = result<float>(imgui_style.PopupRounding),
        .PopupBorderSize = result<float>(imgui_style.PopupBorderSize),
        .FramePadding = std::make_tuple(result<float>(imgui_style.FramePadding.x), result<float>(imgui_style.FramePadding.y)),
        .FrameRounding = result<float>(imgui_style.FrameRounding),
        .FrameBorderSize = result<float>(imgui_style.FrameBorderSize),
        .ItemSpacing = std::make_tuple(result<float>(imgui_style.ItemSpacing.x), result<float>(imgui_style.ItemSpacing.y)),
        .ItemInnerSpacing = std::make_tuple(result<float>(imgui_style.ItemInnerSpacing.x), result<float>(imgui_style.ItemInnerSpacing.y)),
        .TouchExtraPadding = std::make_tuple(result<float>(imgui_style.TouchExtraPadding.x), result<float>(imgui_style.TouchExtraPadding.y)),
        .IndentSpacing = result<float>(imgui_style.IndentSpacing),
        .ColumnsMinSpacing = result<float>(imgui_style.ColumnsMinSpacing),
        .ScrollbarSize = result<float>(imgui_style.ScrollbarSize),
        .ScrollbarRounding = result<float>(imgui_style.ScrollbarRounding),
        .GrabMinSize = result<float>(imgui_style.GrabMinSize),
        .GrabRounding = result<float>(imgui_style.GrabRounding),
        .LogSliderDeadzone = result<float>(imgui_style.LogSliderDeadzone),
        .TabRounding = result<float>(imgui_style.TabRounding),
        .TabBorderSize = result<float>(imgui_style.TabBorderSize),
        .TabMinWidthForUnselectedCloseButton = result<float>(imgui_style.TabMinWidthForUnselectedCloseButton),
        .ColorButtonPosition = {},
        .ButtonTextAlign = std::make_tuple(result<float>(imgui_style.ButtonTextAlign.x), result<float>(imgui_style.ButtonTextAlign.y)),
        .SelectableTextAlign = std::make_tuple(result<float>(imgui_style.SelectableTextAlign.x), result<float>(imgui_style.SelectableTextAlign.y)),
        .DisplayWindowPadding = std::make_tuple(result<float>(imgui_style.DisplayWindowPadding.x), result<float>(imgui_style.DisplayWindowPadding.y)),
        .DisplaySafeAreaPadding = std::make_tuple(result<float>(imgui_style.DisplaySafeAreaPadding.x), result<float>(imgui_style.DisplaySafeAreaPadding.y)),
        .MouseCursorScale = result<float>(imgui_style.MouseCursorScale),
        .AntiAliasedLines = result<bool>(imgui_style.AntiAliasedLines),
        .AntiAliasedLinesUseTex = result<bool>(imgui_style.AntiAliasedLinesUseTex),
        .AntiAliasedFill = result<bool>(imgui_style.AntiAliasedFill),
        .CurveTessellationTol = result<float>(imgui_style.CurveTessellationTol),
        .CircleSegmentMaxError = result<float>(imgui_style.CircleSegmentMaxError)
        //rgba_color      Colors[ImGuiCol_COUNT]{},
    };
    return style;
}

static constexpr int from_direction(const uxx::direction direction) noexcept
{
    switch (direction) {
    case uxx::direction::left:
        return ImGuiDir_Left;
    case uxx::direction::right:
        return ImGuiDir_Right;
    case uxx::direction::up:
        return ImGuiDir_Up;
    case uxx::direction::down:
        return ImGuiDir_Down;
    case uxx::direction::none:
    default:
        return ImGuiDir_None;
    }
}

void uxx::app::set_style(const uxx::style& style)
{
    auto& imgui = ImGui::GetStyle();

    imgui.Alpha = style.Alpha;
    imgui.WindowPadding = { std::get<0>(style.WindowPadding), std::get<1>(style.WindowPadding) };
    imgui.WindowRounding = style.WindowRounding;
    imgui.WindowBorderSize = style.WindowBorderSize;
    imgui.WindowMinSize = { std::get<0>(style.WindowMinSize), std::get<1>(style.WindowMinSize) };
    imgui.WindowTitleAlign = { std::get<0>(style.WindowTitleAlign), std::get<1>(style.WindowTitleAlign) };
    imgui.WindowMenuButtonPosition = from_direction(style.WindowMenuButtonPosition);
    imgui.ChildRounding = style.ChildRounding;
    imgui.ChildBorderSize = style.ChildBorderSize;
    imgui.PopupRounding = style.PopupRounding;
    imgui.PopupBorderSize = style.PopupBorderSize;
    imgui.FramePadding = { std::get<0>(style.FramePadding), std::get<1>(style.FramePadding) };
    imgui.FrameRounding = style.FrameRounding;
    imgui.FrameBorderSize = style.FrameBorderSize;
    imgui.ItemSpacing = { std::get<0>(style.ItemSpacing), std::get<1>(style.ItemSpacing) };
    imgui.ItemInnerSpacing = { std::get<0>(style.ItemInnerSpacing), std::get<1>(style.ItemInnerSpacing) };
    imgui.TouchExtraPadding = { std::get<0>(style.TouchExtraPadding), std::get<1>(style.TouchExtraPadding) };
    imgui.IndentSpacing = style.IndentSpacing;
    imgui.ColumnsMinSpacing = style.ColumnsMinSpacing;
    imgui.ScrollbarSize = style.ScrollbarSize;
    imgui.ScrollbarRounding = style.ScrollbarRounding;
    imgui.GrabMinSize = style.GrabMinSize;
    imgui.GrabRounding = style.GrabRounding;
    imgui.LogSliderDeadzone = style.LogSliderDeadzone;
    imgui.TabRounding = style.TabRounding;
    imgui.TabBorderSize = style.TabBorderSize;
    imgui.TabMinWidthForUnselectedCloseButton = style.TabMinWidthForUnselectedCloseButton;
    imgui.ColorButtonPosition = from_direction(style.ColorButtonPosition);
    imgui.ButtonTextAlign = { std::get<0>(style.ButtonTextAlign), std::get<1>(style.ButtonTextAlign) };
    imgui.SelectableTextAlign = { std::get<0>(style.SelectableTextAlign), std::get<1>(style.SelectableTextAlign) };
    imgui.DisplayWindowPadding = { std::get<0>(style.DisplayWindowPadding), std::get<1>(style.DisplayWindowPadding) };
    imgui.DisplaySafeAreaPadding = { std::get<0>(style.DisplaySafeAreaPadding), std::get<1>(style.DisplaySafeAreaPadding) };
    imgui.MouseCursorScale = style.MouseCursorScale;
    imgui.AntiAliasedLines = style.AntiAliasedLines;
    imgui.AntiAliasedLinesUseTex = style.AntiAliasedLinesUseTex;
    imgui.AntiAliasedFill = style.AntiAliasedFill;
    imgui.CurveTessellationTol = style.CurveTessellationTol;
    imgui.CircleSegmentMaxError = style.CircleSegmentMaxError;
    //rgba_color      Colors[ImGuiCol_COUNT];
}
