#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Scene/Camera/Camera.hpp>

namespace vke {

class UniformCamera : public VulkanObject
{
    public:
        UniformCamera(VulkanContext *context);
        ~UniformCamera() override = default;

        void update() override;

    protected:
        Camera _camera;

        maths::Vector3f _rotation = {};
        maths::Vector3f _position = {};
        f32 _zoom = 1.f;

        vke::VulkanBuffer _uniform_buffer;
};

}// namespace vke
