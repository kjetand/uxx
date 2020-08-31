#include <uxx/uxx.hpp>

int main()
{
    uxx::app app;
    return app.run([](auto& canvas, uxx::string_ref title) {
        canvas.window(title, [](auto& /*ignored*/) {});
        return 0;
    },
        "Hello, world!");
}