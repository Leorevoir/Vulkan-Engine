#include <VKE/Core/Time.hpp>

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
    const auto time_end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();

    _delta_time = static_cast<f32>(duration) / 1000.f;
}

f32 vke::core::Time::getDeltaTime() const
{
    return _delta_time;
}

f32 vke::core::Time::toMilliseconds(f32 seconds)
{
    return seconds * 1000.f;
}

f32 vke::core::Time::toSeconds(f32 milliseconds)
{
    return milliseconds / 1000.f;
}
