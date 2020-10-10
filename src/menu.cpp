#include "common.hpp"
#include "uxx/uxx.hpp"

bool uxx::menu::item(string_ref label) const
{
    return ImGui::MenuItem(label);
}
void uxx::menu::separator() const
{
    ImGui::Separator();
}
