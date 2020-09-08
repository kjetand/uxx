#include <uxx/uxx.hpp>

int main()
{
    uxx::app app;
    return app.run([](auto& canvas, auto title) {
        canvas.window(title, [](auto& /*ignored*/) {});
    },
        "Hello, world!");
}