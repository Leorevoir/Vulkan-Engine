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
        maths::Vector3f _target_rotation = {};
        f32 _target_zoom = 5.f;
        f32 _camera_zoom = 5.f;
        f32 _camera_distance = 5.f;

    private:
        event::MousePosition _old_position = {};

        void _third_person_camera_rotation(const f32 dt);
        void _third_person_camera_interpolation(const f32 dt);
        void _third_person_camera_zoom(const f32 dt);
};

}// namespace vke
