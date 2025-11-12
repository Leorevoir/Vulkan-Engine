#pragma once

#include <vulkan_backend/pipeline/Pipeline.hpp>

#include <memory>
#include <vector>

namespace lumen {

class RenderWindow;
class RenderObject;
class GraphicsContext;
class Renderer;

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

        std::vector<RenderObject> _render_objects;

    private:
        void _main_loop();
        void _wait_for_delete();

        std::unique_ptr<RenderWindow> _window;
        std::unique_ptr<GraphicsContext> _context;
        std::unique_ptr<Renderer> _renderer;
        std::unique_ptr<Pipeline> _pipeline;
};

}// namespace lumen
