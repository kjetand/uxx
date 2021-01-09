#include "common.hpp"
#include "uxx/uxx.hpp"

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
#include <vlc/vlc.h>

#include <array>
#include <memory>
#include <span>

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

    [[nodiscard]] std::pair<unsigned int, unsigned int> get_resolution() const
    {
        if (nullptr == _media) {
            return { 0, 0 };
        }

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
        if (!libvlc_media_is_parsed(_media.get())) {
            libvlc_media_parse(_media.get());
        }
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

        unsigned int width { 0 };
        unsigned int height { 0 };

        libvlc_video_get_size(_player.get(), 0, &width, &height);

        if (width != 0 && height != 0) {
            return { width, height };
        }
        libvlc_media_track_t** info;
        const auto info_count = libvlc_media_tracks_get(_media.get(), &info);

        std::span<libvlc_media_track_t*> info_span(info, info_count);

        for (const auto* track : info_span) {
            const auto* video_info { track->video };

            if (libvlc_track_video == track->i_type && video_info->i_width && video_info->i_height) {
                width = video_info->i_width;
                height = video_info->i_height;
                break;
            }
        }
        libvlc_media_tracks_release(info, info_count);
        return { width, height };
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
    , _width(0.0f)
    , _height(0.0f)
{
}

uxx::video::~video()
{
}

void uxx::video::load_from_disk(const std::filesystem::path& video_path)
{
    _driver->load_from_disk(video_path);

    _raw_image->texture = {};
    const auto [width, height] = _driver->get_resolution();

    if (width == 0 || height == 0) {
        throw std::runtime_error("Unable to get resolution of video");
    }
    _width = static_cast<float>(width);
    _height = static_cast<float>(height);

    _raw_image->texture.create(width, height);
    _raw_frame.resize(width * height * 4);

    _driver->set_output(_raw_frame.data(), width, height);
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
    return uxx::width { _width };
}

uxx::height uxx::video::get_height() const noexcept
{
    return uxx::height { _height };
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

std::pair<uxx::width, uxx::height> uxx::video::get_resolution() const
{
    const auto [width, height] = _driver->get_resolution();
    return { uxx::width { static_cast<float>(width) }, uxx::height { static_cast<float>(height) } };
}
