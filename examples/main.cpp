#include <uxx/uxx.hpp>

int main()
{
    uxx::app app;
    app.run([](auto& canvas, auto title) {
        canvas.window(title, [](auto& /*ignored*/) {});
    },
        "Hello, world!");
    return 0;
}