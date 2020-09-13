#include "common.hpp"
#include "uxx/uxx.hpp"

// TODO: Consider method signature (hint: bool as parameter?)
bool uxx::popup::menu_item(string_ref label, const bool enabled) const
{
    return ImGui::MenuItem(label, nullptr, false, enabled);
}
