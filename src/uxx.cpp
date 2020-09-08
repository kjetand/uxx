#include "uxx/uxx.hpp"

#ifdef _WIN32
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#pragma GCC diagnostic pop
#endif

uxx::string_ref::string_ref(const std::string& str) noexcept
    : _str(str.c_str())
{
}

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

void uxx::canvas::begin_window(uxx::string_ref title) const
{
    ImGui::Begin(title);
}

void uxx::canvas::end_window() const
{
    ImGui::End();
}
