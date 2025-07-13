#pragma once

#include <VK/Renderer/RenderEngine.hpp>
#include <VK/Types.hpp>
#include <VK/Window/Window.hpp>

namespace vk {

namespace priv {

class VK_HIDDEN RenderLoop final
{
    public:
        explicit RenderLoop(const f64 frame_rate, render::Engine &engine);
        ~RenderLoop();

        void start(vk::Window &window);
        void shutdown();

    private:
        render::Engine &_engine;
        bool _is_running;
        f64 _frame_time;

        void _run(vk::Window &window);
        void _destroy();
};

}// namespace priv

}// namespace vk
