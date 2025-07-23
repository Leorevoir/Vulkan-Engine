#include <VKE/Core/Time.hpp>

#include <chrono>
#include <thread>

/**
* public
*/

static auto time_start = std::chrono::high_resolution_clock::now();

vke::core::Time &vke::core::Time::getInstance()
{
    static Time instance;
    return instance;
}

void vke::core::Time::start()
{
    time_start = std::chrono::high_resolution_clock::now();
}

void vke::core::Time::stop()
{
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<f32> duration = time_end - time_start;

    const f32 frame_time = duration.count();

    if (_max_fps > 0.f) {
        const f32 target_time = 1.f / _max_fps;
        const f32 sleep_duration = target_time - frame_time;

        if (sleep_duration > 0.f) {
            std::this_thread::sleep_for(std::chrono::duration<f32>(sleep_duration));

            time_end = std::chrono::high_resolution_clock::now();
            duration = time_end - time_start;
        }
    }

    _delta_time = duration.count();
}

/**
* getters
*/

f32 vke::core::Time::getDeltaTime() const
{
    return _delta_time;
}

f32 vke::core::Time::getFPS() const
{
    return (_delta_time > 0.f) ? (1.f / _delta_time) : 0.f;
}

f32 vke::core::Time::toMilliseconds(f32 seconds)
{
    return seconds * 1000.f;
}

f32 vke::core::Time::toSeconds(f32 milliseconds)
{
    return milliseconds / 1000.f;
}

/**
* setters
*/

void vke::core::Time::setMaxFPS(const f32 max_fps)
{
    _max_fps = max_fps > 0.f ? max_fps : VKE_DEFAULT_MAX_FPS;
}
