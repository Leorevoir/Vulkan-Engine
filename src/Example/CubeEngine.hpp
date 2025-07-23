#pragma once

#include "VKE/Graphics/Engine/GameEngine.hpp"
#include "VKE/Graphics/Shader/UniformCamera.hpp"
#include "VKE/Graphics/Shader/VulkanShader.hpp"
#include "VKE/Meshes/MeshCube.hpp"

#include <memory>

class CubeEngine final : public vke::GameEngine
{
    public:
        CubeEngine() = default;
        ~CubeEngine() override = default;

        void initialize() override;
        void update() override;
        void drawObjects(VkCommandBuffer &cmd) override;

    private:
        std::shared_ptr<vke::MeshCube> _mesh;
        std::shared_ptr<vke::VulkanShader> _shader;
        std::shared_ptr<vke::UniformCamera> _uniform;
};
