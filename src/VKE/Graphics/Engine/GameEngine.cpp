#include <VKE/Core/Time.hpp>
#include <VKE/Graphics/Engine/GameEngine.hpp>
#include <iostream>

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
        std::cout << "FPS: " << core::Time::getInstance().getFPS() << std::endl;
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
