#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/Shader/Base/VulkanUniformBase.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Scene/Camera/Camera.hpp>
#include <VKE/Scene/Camera/ThirdPersonCamera.hpp>

namespace vke {

class VKE_API UniformCamera : public priv::VulkanUniformBase, public VulkanObject, public ThirdPersonCamera
{
    public:
        constexpr UniformCamera() = default;
        ~UniformCamera() override;

        void initialize() override;
        void update() override;

    protected:
        void updateCameraMatrix();

        Camera _camera;
};

}// namespace vke
