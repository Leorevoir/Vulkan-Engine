#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/Shader/VulkanUniform.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Scene/Camera/Camera.hpp>

namespace vke {

class VKE_API UniformCamera : public VulkanUniform, public VulkanObject
{
    public:
        constexpr UniformCamera() = default;
        ~UniformCamera() override = default;

        void initialize() override;
        void update() override;

        void setRotation(maths::Vector3f *rotation);
        void setPosition(maths::Vector3f *position);
        void setZoom(f32 *zoom);

    protected:
        Camera _camera;

        maths::Vector3f *_rotation = nullptr;
        maths::Vector3f *_position = nullptr;
        f32 *_zoom = nullptr;
};

}// namespace vke
