#include <VKE/Renderer/RenderLoop.hpp>
#include <thread>

/**
* public
*/

vke::priv::RenderLoop::RenderLoop(const f64 frame_rate, RenderEngine &engine) : _engine(engine), _is_running(false), _frame_time(1.0 / frame_rate)
{
    _engine.init();
}

vke::priv::RenderLoop::~RenderLoop()
{
    shutdown();
}

void vke::priv::RenderLoop::start(Window &window)
{
    if (_is_running) {
        return;
    }
    _is_running = true;
    _engine.start();
    _run(window);
}

void vke::priv::RenderLoop::shutdown()
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

void vke::priv::RenderLoop::_run(Window &window)
{
    while (!window.shouldClose() && _is_running) {
        window.pollEvents();
        _engine.render();
        window.flush();
    }
}

void vke::priv::RenderLoop::_destroy()
{
    _engine.shutdown();
}
