#include "common.hpp"
#include "uxx/uxx.hpp"

uxx::pane::collapsed uxx::screen::begin_window(uxx::string_ref title) const
{
    return ImGui::Begin(title) ? pane::collapsed::no : pane::collapsed::yes;
}

uxx::pane::collapsed uxx::screen::begin_window(uxx::string_ref title, uxx::result<bool>& open, uxx::pane::properties properties) const
{
    return ImGui::Begin(title, &open.get(), static_cast<int>(properties)) ? pane::collapsed::no : pane::collapsed::yes;
}

void uxx::screen::end_window() const
{
    ImGui::End();
}

void uxx::screen::begin_main_menu_bar() const
{
    ImGui::BeginMainMenuBar();
}

void uxx::screen::end_main_menu_bar() const
{
    ImGui::EndMainMenuBar();
}
