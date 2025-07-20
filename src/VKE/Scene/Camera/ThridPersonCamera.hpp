#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Maths/Vector.hpp>

namespace vke {

class VKE_API ThirdPersonCamera
{
    public:
        constexpr ThirdPersonCamera() = default;
        virtual ~ThirdPersonCamera() = default;

    protected:
        maths::Vector3f _camera_position = {};
        maths::Vector3f _camera_rotation = {};
        f32 _camera_zoom = -2.f;
};

}// namespace vke
