#include "common.hpp"
#include "uxx/uxx.hpp"

struct uxx::image::data {
    sf::Texture texture {};
};

uxx::image::image(const std::filesystem::path& image_path) noexcept
    : _texture { std::make_unique<data>() }
{
    auto path = image_path.generic_string();

    if (!_texture->texture.loadFromFile(path)) {
        _texture = nullptr;
    }
}

uxx::image::~image() noexcept
{
}

unsigned int uxx::image::get_width() const noexcept
{
    if (_texture) {
        return _texture->texture.getSize().x;
    }
    return 0;
}

unsigned int uxx::image::get_height() const noexcept
{
    if (_texture) {
        return _texture->texture.getSize().y;
    }
    return 0;
}

unsigned int uxx::image::get_native_handle() const
{
    return _texture->texture.getNativeHandle();
}
