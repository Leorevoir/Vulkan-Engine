#include <Application.hpp>

#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/pipeline/Pipeline.hpp>
#include <vulkan_backend/renderer/RenderWindow.hpp>
#include <vulkan_backend/renderer/Renderer.hpp>
#include <vulkan_backend/utils/Config.hpp>

/**
* public
*/

lumen::Application::Application()
{
    _window = std::make_unique<RenderWindow>("Lumen Engine", config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    _context = std::make_unique<GraphicsContext>(*_window);
    _renderer = std::make_unique<Renderer>(*_context, *_window);
    _create_render_assets();
}

lumen::Application::~Application()
{
    vkDeviceWaitIdle(_context->get_device().logicalDevice());
}

void lumen::Application::run()
{
    _main_loop();
}

/**
* private
*/

void lumen::Application::_create_render_assets()
{
    _pipeline = std::make_unique<Pipeline>(_context->get_device(), _renderer->get_render_pass(), "assets/shaders/triangle.vert.spv",
        "assets/shaders/triangle.frag.spv");
}

void lumen::Application::_main_loop()
{
    while (!_window->should_close()) {
        _window->poll_events();
        _renderer->draw_frame(*_pipeline);
    }
}
