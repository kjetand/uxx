#include "common.hpp"
#include "uxx/uxx.hpp"

bool uxx::menu_bar::begin_menu(uxx::string_ref label) const
{
    return ImGui::BeginMenu(label);
}

void uxx::menu_bar::end_menu() const
{
    ImGui::EndMenu();
}
