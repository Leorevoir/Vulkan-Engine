#include <VK/Renderer/RenderLoop.hpp>
#include <thread>

/**
* public
*/

vk::priv::RenderLoop::RenderLoop(const f64 frame_rate, render::Engine &engine) : _engine(engine), _is_running(false), _frame_time(1.0 / frame_rate)
{
    _engine.init();
}

vk::priv::RenderLoop::~RenderLoop()
{
    shutdown();
}

void vk::priv::RenderLoop::start(vk::Window &window)
{
    if (_is_running) {
        return;
    }
    _is_running = true;
    _engine.start();
    _run(window);
}

void vk::priv::RenderLoop::shutdown()
{
    if (!_is_running) {
        return;
    }
    _is_running = false;
    _destroy();
}

/**
* private
*/

void vk::priv::RenderLoop::_run(vk::Window &window)
{
    while (!window.shouldClose() && _is_running) {
        window.pollEvents();
        _engine.render();
        window.flush();
    }
}

void vk::priv::RenderLoop::_destroy()
{
    _engine.shutdown();
}
