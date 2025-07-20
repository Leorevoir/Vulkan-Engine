#include <VKE/Graphics/Engine/GameEngine.hpp>

#include <chrono>
#include <iostream>
#include <thread>

/**
* public
*/

void vke::GameEngine::start()
{
    std::cout << "starting..." << std::endl;
    priv::VulkanEngineBase::start();
    initialize();
    build_command_buffer();
    _prepared = true;
    _running = true;
    std::cout << "started!" << std::endl;
}

void vke::GameEngine::stop()
{
    _running = false;
}

void vke::GameEngine::renderLoop()
{
    std::cout << "flushing window..." << std::endl;
    _window->flush();
    std::cout << "window flushed!" << std::endl;

    std::cout << "starting render loop..." << std::endl;
    while (_running) {

        if (_window->shouldClose()) {
            return stop();
        }

        renderFrame();

        //TODO: real timing bc this sucks
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    std::cout << "render loop stopped!" << std::endl;
}

void vke::GameEngine::update()
{
    _window->pollEvents();
}
