#pragma once

#include "VKE/Graphics/Engine/GameEngine.hpp"
#include "VKE/Graphics/Shader/UniformCamera.hpp"
#include "VKE/Graphics/Shader/VulkanShader.hpp"
#include "VKE/Meshes/MeshObject.hpp"

// class Triangle : public vke::MeshObject
// {
//     public:
//         Triangle();
//         ~Triangle() override = default;
// };
//
// class TriangleShader : public vke::VulkanShader
// {
//     public:
//         TriangleShader();
//         ~TriangleShader() = default;
// };

class TriangleEngine : public vke::GameEngine
{
    public:
    protected:
        void initialize() override;

    private:
        // std::shared_ptr<Triangle> _triangle = nullptr;
        // std::shared_ptr<TriangleShader> _triangle_shader = nullptr;
        // std::shared_ptr<vke::UniformCamera> _triangle_uniform = nullptr;
};
