#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::tab_bar::visible uxx::pane::begin_tab_bar(uxx::id<string_ref> id) const
{
    return ImGui::BeginTabBar(id.get()) ? tab_bar::visible::yes : tab_bar::visible::no;
}

void uxx::pane::end_tab_bar() const
{
    ImGui::EndTabBar();
}

uxx::tab_bar::tab_bar(pane& w) noexcept
    : _window(w)
{
}

bool uxx::tab_bar::begin_tab_item(string_ref label) const
{
    return ImGui::BeginTabItem(label);
}

void uxx::tab_bar::end_tab_item() const
{
    ImGui::EndTabItem();
}
