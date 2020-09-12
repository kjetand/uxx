#include <uxx/uxx.hpp>

int main()
{
    uxx::app app;
    return app.run([](auto& scene, auto title) {
        static bool open = true;
        static const auto properties = uxx::window::properties {};

        scene.window(title, open, properties, [](auto& w) {
            if (w.is_collapsed()) {
                return;
            }
            auto pencil = w.create_pencil();
            pencil.draw_line({ 10, 10 }, { 100, 100 });

            w.label("Enter your name: ");
            w.same_line();

            static std::string name {};
            w.input_text("", name);

            if (w.button("Submit")) {
                std::puts("Your name is: ");
                std::puts(name.c_str());
            }
        });
    },
        "Hello, world!");
}