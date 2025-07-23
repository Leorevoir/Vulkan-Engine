#pragma once

#include <VKE/Types.hpp>
#include <chrono>

namespace vke {

namespace core {

class Time final : public NonCopyable
{
    public:
        static Time &getInstance();

        void start();
        void stop();

        f32 getDeltaTime() const;

        static f32 toMilliseconds(f32 seconds);
        static f32 toSeconds(f32 milliseconds);

    private:
        f32 _delta_time;
};

}// namespace core

}// namespace vke
