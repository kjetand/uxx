#include "common.hpp"
#include "uxx/uxx.hpp"

void uxx::app::set_width(unsigned int width) noexcept
{
    _width = width;
}

void uxx::app::set_height(unsigned int height) noexcept
{
    _height = height;
}

void uxx::app::mainloop(string_ref title, const std::function<void()>& render) const
{
    constexpr bool load_default_font = false;

    sf::RenderWindow w(sf::VideoMode(_width, _height), title.c_str());
    w.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(w, load_default_font);
    auto& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 15.0f);
    ImGui::SFML::UpdateFontTexture();

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
