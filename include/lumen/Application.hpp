#pragma once

#include <memory>

namespace lumen {

class RenderWindow;
class Renderer;
class GraphicsContext;

class Application
{
    public:
        Application();
        virtual ~Application();

        void run();

    protected:
        virtual void on_create() = 0;
        virtual void on_update() = 0;
        virtual void on_destroy() = 0;

        RenderWindow &get_window() const noexcept;
        GraphicsContext &get_context() const noexcept;
        Renderer &get_renderer() const noexcept;

    private:
        void _main_loop();

        std::unique_ptr<RenderWindow> _window;
        std::unique_ptr<GraphicsContext> _context;
        std::unique_ptr<Renderer> _renderer;
};

}// namespace lumen
