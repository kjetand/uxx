#include <uxx/uxx.hpp>

int main()
{
    uxx::app app;
    return app.run([](auto& canvas, auto title) {
        static bool open = true;
        static const auto properties = uxx::window::properties {}.set_no_resize();

        canvas.window(title, open, properties, [](uxx::window& w) {
            if (w.is_collapsed()) {
                return;
            }
            w.label("Enter your name:");
            w.same_line();

            if (w.button("Submit")) {
                std::puts("Your name is: ");
            }
        });
    },
        "Hello, world!");
}