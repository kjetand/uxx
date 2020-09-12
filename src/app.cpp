#include "uxx/uxx.hpp"
#include "common.hpp"

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
