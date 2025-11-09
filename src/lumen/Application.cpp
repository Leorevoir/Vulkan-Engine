#include <lumen/Application.hpp>

#include <vulkan_backend/core/GraphicsContext.hpp>
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
}

lumen::Application::~Application()
{
    if (_context) {
        vkDeviceWaitIdle(_context->get_device().logicalDevice());
    }
}

void lumen::Application::run()
{
    on_create();
    _main_loop();
    on_destroy();
}

/**
 * protected
 */

/**
 * getters
 */

lumen::RenderWindow &lumen::Application::get_window() const noexcept
{
    return *_window;
}

lumen::GraphicsContext &lumen::Application::get_context() const noexcept
{
    return *_context;
}

lumen::Renderer &lumen::Application::get_renderer() const noexcept
{
    return *_renderer;
}

/**
* private
*/

void lumen::Application::_main_loop()
{
    while (!_window->should_close()) {
        _window->poll_events();
        on_update();
    }
}
