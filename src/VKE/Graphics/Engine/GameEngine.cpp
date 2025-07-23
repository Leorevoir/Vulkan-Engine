#include <VKE/Graphics/Engine/GameEngine.hpp>

#include <chrono>
#include <thread>

/**
* public
*/

void vke::GameEngine::initialize()
{
    /* __initialize__ */
}

void vke::GameEngine::start()
{
    priv::VulkanEngineBase::start();
    initialize();
    build_command_buffer();
    _prepared = true;
    _running = true;
}

void vke::GameEngine::stop()
{
    _running = false;
}

void vke::GameEngine::renderLoop()
{
    _window->flush();

    while (_running) {

        if (_window->shouldClose()) {
            return stop();
        }

        _window->pollEvents();
        renderFrame();

        //TODO: real timing bc this sucks
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (_device != VKE_NULLPTR) {
        vkDeviceWaitIdle(_device);
    }
}

void vke::GameEngine::draw()
{
    /* __draw__ */
}

void vke::GameEngine::update()
{
    /* __update__ */
}
