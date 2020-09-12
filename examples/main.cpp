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
            uxx::pencil pencil = w.create_pencil();
            pencil.set_thickness(5.f);
            pencil.draw_line({ 10, 10 }, { 100, 100 });
            pencil.set_properties(uxx::pencil::properties {}.set_anti_aliased_lines().set_anti_aliased_fill());
            pencil.draw_polyline({ { 10, 10 }, { 100, 100 }, { 120, 300 }, { 150, 120 } }, false);
            pencil.draw_bezier_curve({10, 100}, {50, 400}, {100, 150}, {100, 200});

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