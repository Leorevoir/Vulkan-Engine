#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Camera/Camera.hpp>
#include <VKE/Camera/ThirdPersonCamera.hpp>
#include <VKE/Graphics/Shader/Base/VulkanUniformBase.hpp>
#include <VKE/Graphics/VulkanObject.hpp>

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
