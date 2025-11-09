#include <Application.hpp>

#include <vulkan_backend/renderer/RenderWindow.hpp>
#include <vulkan_backend/renderer/Renderer.hpp>
#include <vulkan_backend/utils/Config.hpp>

/**
* public
*/

lumen::Application::Application()
{
    _window = std::make_unique<RenderWindow>("Lumen Engine", config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
    _renderer = std::make_unique<Renderer>(*_window);
}

lumen::Application::~Application()
{
    /* __dtor__ */
}

void lumen::Application::run()
{
    _main_loop();
}

/**
* private
*/

void lumen::Application::_main_loop()
{
    while (!_window->should_close()) {
        _window->poll_events();
        _renderer->draw_frame();
    }
}
