#include "uxx/uxx.hpp"

#ifdef _WIN32
#include <SFML/Graphics.hpp>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#include <SFML/Graphics.hpp>
#pragma GCC diagnostic pop
#endif

uxx::string_ref::string_ref(const std::string& str) noexcept
    : _str(str.c_str())
{
}
void uxx::app::mainloop() const
{
    sf::RenderWindow w(sf::VideoMode(800, 600), "SFML window");
    while (w.isOpen()) {
        sf::Event event;
        while (w.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                w.close();
        }
        w.clear();
        w.display();
    }
}
