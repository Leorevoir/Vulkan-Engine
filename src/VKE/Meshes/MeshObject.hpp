#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Maths/Vector.hpp>

#include <memory>

namespace vke {

class VulkanShader;
class VulkanBuffer;
class UniformCamera;

class VKE_API MeshObject : public VulkanObject
{
    public:
        virtual ~MeshObject();

        /** after vulkan */
        /* + initialize() */
        /* + update() */
        void bind(VkCommandBuffer &cmd_buffer, std::shared_ptr<VulkanShader> vulkan_shader);
        void bind(VkCommandBuffer &cmd_buffer);

        /** getters */
        const maths::Vector3f &getPosition() const;
        const maths::Vector3f &getRotation() const;
        const maths::Vector3f &getScale() const;

        /** setters */
        void setPosition(const maths::Vector3f &position);
        void setRotation(const maths::Vector3f &rotation);
        void setScale(const maths::Vector3f &scale);
        void setColor(const maths::Vector4f &color);

    protected:
        maths::Vector3f _position = {};
        maths::Vector3f _rotation = {};
        maths::Vector3f _scale = {1.0f, 1.0f, 1.0f};
        maths::Color _color = {0.f, 1.f, 0.f, 1.f};

        u32 _index_count = 0;

        VulkanBuffer _vertex_buffer;
        VulkanBuffer _index_buffer;
        VulkanBuffer _color_buffer;

        std::shared_ptr<UniformCamera> _uniform = VKE_NULLPTR;
        std::shared_ptr<VulkanShader> _shader = VKE_NULLPTR;
};

}// namespace vke
