#pragma once

#include "VKE/Graphics/Engine/GameEngine.hpp"
#include "VKE/Graphics/Shader/UniformCamera.hpp"
#include "VKE/Graphics/Shader/VulkanShader.hpp"
#include "VKE/Meshes/MeshObject.hpp"
#include "VKE/Scene/Camera/ThridPersonCamera.hpp"

class Triangle : public vke::MeshObject
{
    public:
        constexpr Triangle() = default;
        ~Triangle() override = default;

        void initialize() override;
        void update() override;
};

class TriangleShader : public vke::VulkanShader
{
    public:
        constexpr TriangleShader() = default;
        ~TriangleShader() = default;

        void initialize() override;
};

class TriangleEngine : public vke::GameEngine, public vke::ThirdPersonCamera
{
    public:
    protected:
        void initialize() override;
        void update() override;
        void drawObjects(VkCommandBuffer &cmd) override;

    private:
        std::shared_ptr<Triangle> _triangle = nullptr;
        std::shared_ptr<TriangleShader> _triangle_shader = nullptr;
        std::shared_ptr<vke::UniformCamera> _triangle_uniform = nullptr;
};
