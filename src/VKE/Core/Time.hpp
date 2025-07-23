#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace core {

class VKE_API Time final : public NonCopyable
{
    public:
        static Time &getInstance();

        void start();
        void stop();

        /** getters */
        f32 getDeltaTime() const;
        f32 getFPS() const;

        /** setters */
        void setMaxFPS(const f32 max_fps);

        static f32 toMilliseconds(const f32 seconds);
        static f32 toSeconds(const f32 milliseconds);

    private:
        f32 _max_fps = VKE_DEFAULT_MAX_FPS;
        f32 _delta_time = .0f;
};

}// namespace core

}// namespace vke
