#pragma once

#include <VKE/Renderer/RenderEngine.hpp>
#include <VKE/Types.hpp>
#include <VKE/Window/Window.hpp>

namespace vke {

namespace priv {

class VKE_HIDDEN RenderLoop final
{
    public:
        explicit RenderLoop(const f64 frame_rate, RenderEngine &engine);
        ~RenderLoop();

        void start(Window &window);
        void shutdown();

    private:
        RenderEngine &_engine;
        bool _is_running;
        f64 _frame_time;

        void _run(Window &window);
        void _destroy();
};

}// namespace priv

}// namespace vke
