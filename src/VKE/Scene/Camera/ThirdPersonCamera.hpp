#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Maths/Vector.hpp>
#include <VKE/Systems/EventTypes.hpp>

namespace vke {

class VKE_API ThirdPersonCamera
{
    public:
        constexpr ThirdPersonCamera() = default;
        virtual ~ThirdPersonCamera() = default;

    protected:
        void updateThirdPersonCamera();

        maths::Vector3f _camera_position = {};
        maths::Vector3f _camera_rotation = {};
        f32 _camera_zoom = -2.f;
        f32 _camera_distance = 5.f;

    private:
        event::MousePosition _old_position = {};
};

}// namespace vke
