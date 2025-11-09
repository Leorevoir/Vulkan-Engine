#pragma once

#include <memory>

namespace lumen {

class RenderWindow;
class Renderer;
class GraphicsContext;
class Pipeline;

class Application
{
    public:
        Application();
        ~Application();

        void run();

    private:
        void _create_render_assets();
        void _main_loop();

        std::unique_ptr<RenderWindow> _window;
        std::unique_ptr<GraphicsContext> _context;
        std::unique_ptr<Renderer> _renderer;
        std::unique_ptr<Pipeline> _pipeline;
};

}// namespace lumen
