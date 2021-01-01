#include "common.hpp"
#include "uxx/uxx.hpp"

struct uxx::image::raw_image {
    sf::Texture texture {};
};

uxx::image::image(const std::filesystem::path& image_path) noexcept
    : _raw_image { std::make_unique<raw_image>() }
{
    auto path = image_path.generic_string();

    if (!_raw_image->texture.loadFromFile(path)) {
        _raw_image = nullptr;
    }
}

uxx::image::~image() noexcept
{
}

float uxx::image::get_width() const noexcept
{
    if (_raw_image) {
        return static_cast<float>(_raw_image->texture.getSize().x);
    }
    return {};
}

float uxx::image::get_height() const noexcept
{
    if (_raw_image) {
        return static_cast<float>(_raw_image->texture.getSize().y);
    }
    return {};
}

std::optional<unsigned int> uxx::image::get_native_handle() const
{
    if (nullptr != _raw_image) {
        return _raw_image->texture.getNativeHandle();
    }
    return {};
}
