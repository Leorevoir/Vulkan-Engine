#pragma once

#include <memory>

namespace lumen {

class RenderWindow;
class Renderer;

class Application
{
    public:
        Application();
        ~Application();

        void run();

    private:
        void _main_loop();

        std::unique_ptr<RenderWindow> _window;
        std::unique_ptr<Renderer> _renderer;
};

}// namespace lumen
