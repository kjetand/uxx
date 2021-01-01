#include "common.hpp"
#include "uxx/uxx.hpp"

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
#include <vlc/vlc.h>

#include <array>
#include <memory>

struct uxx::video::raw_image {
    sf::Texture texture {};
};

// Thin LibVLC wrapper
class uxx::video::driver {
    struct instance_deleter {
        void operator()(libvlc_instance_t* ptr) noexcept
        {
            libvlc_release(ptr);
        }
    };

    struct player_deleter {
        void operator()(libvlc_media_player_t* ptr) noexcept
        {
            libvlc_media_player_release(ptr);
        }
    };

    struct media_deleter {
        void operator()(libvlc_media_t* ptr) noexcept
        {
            libvlc_media_release(ptr);
        }
    };

    using instance_type = std::unique_ptr<libvlc_instance_t, instance_deleter>;
    using player_type = std::unique_ptr<libvlc_media_player_t, player_deleter>;
    using media_type = std::unique_ptr<libvlc_media_t, media_deleter>;

#ifdef NDEBUG
    static constexpr std::array VLC_ARGS { "--verbose=0" };
#else
    static constexpr std::array VLC_ARGS { "--verbose=1" };
#endif

public:
    explicit driver()
        : _instance(libvlc_new(static_cast<int>(VLC_ARGS.size()), VLC_ARGS.data()))
        , _player(nullptr)
        , _media(nullptr)
    {
        if (nullptr == _instance) {
            throw std::runtime_error("Unable to initialize video backend");
        }
        _player = player_type(libvlc_media_player_new(_instance.get()));

        if (nullptr == _player) {
            throw std::runtime_error("Unable to create video driver");
        }
    }

    ~driver() = default;

    void load_from_disk(const std::filesystem::path& video_path)
    {
        _media = media_type(libvlc_media_new_path(_instance.get(), video_path.string().c_str()));

        if (nullptr == _media) {
            throw std::runtime_error("Failed to load media from disk");
        }
        libvlc_media_player_set_media(_player.get(), _media.get());
    }

    void set_output(unsigned char* output, const unsigned int width, const unsigned int height) noexcept
    {
        libvlc_video_set_callbacks(
            _player.get(), lock, unlock, display, output);
        libvlc_video_set_format(
            _player.get(), "RGBA", width, height, width * 4);
    }

    void play() noexcept
    {
        libvlc_media_player_play(_player.get());
    }

    void pause() noexcept
    {
        libvlc_media_player_pause(_player.get());
    }

    void stop() noexcept
    {
        libvlc_media_player_stop(_player.get());
    }

    [[nodiscard]] bool is_loaded() const noexcept
    {
        return nullptr != _media;
    }

private:
    instance_type _instance;
    player_type _player;
    media_type _media;

    struct raw_data {
        unsigned char* frame { nullptr };
    };

    static void* lock(void* data, void** pixels)
    {
        auto* raw_data = static_cast<struct raw_data*>(data);
        *pixels = raw_data;
        return nullptr;
    }

    static void unlock(void*, void*, void* const*)
    {
    }

    static void display(void*, void*)
    {
    }
};

uxx::video::video()
    : _raw_frame()
    , _driver(std::make_unique<uxx::video::driver>())
    , _raw_image(std::make_unique<uxx::video::raw_image>())
{
}

uxx::video::~video()
{
}

void uxx::video::load_from_disk(const std::filesystem::path& video_path)
{
    _driver->load_from_disk(video_path);

    _raw_image->texture = {};
    _raw_image->texture.create(400, 400); // TODO: Get resolution
    _raw_frame.resize(400 * 400 * 4);

    _driver->set_output(_raw_frame.data(), 400, 400);
}

void uxx::video::play() noexcept
{
    _driver->play();
}

void uxx::video::stop() noexcept
{
    _driver->stop();
}

void uxx::video::pause() noexcept
{
    _driver->pause();
}

bool uxx::video::is_loaded() const noexcept
{
    return _driver->is_loaded();
}

uxx::width uxx::video::get_width() const noexcept
{
    return uxx::width { 400 };
}

uxx::height uxx::video::get_height() const noexcept
{
    return uxx::height { 400 };
}

std::optional<unsigned int> uxx::video::get_native_handle() const
{
    if (nullptr != _raw_image) {
        return _raw_image->texture.getNativeHandle();
    }
    return {};
}

void uxx::video::render() const
{
    if (nullptr != _raw_image) {
        _raw_image->texture.update(_raw_frame.data());
    }
}
