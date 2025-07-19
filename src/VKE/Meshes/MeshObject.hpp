#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Maths/Vector.hpp>

#include <VKE/Graphics/Device/VulkanBuffer.hpp>
#include <VKE/Graphics/Shader/VulkanShader.hpp>

namespace vke {

class VKE_API MeshObject : public VulkanObject
{
    public:
        MeshObject();
        virtual ~MeshObject();

        /** after vulkan */
        void build(VkCommandBuffer &cmd_buffer, VulkanShader *vulkan_shader);

        virtual void generate() = 0;
        void update() = 0;

        /** getters */
        const maths::Vector3f &getPosition() const;
        const maths::Vector3f &getRotation() const;
        const maths::Vector3f &getScale() const;

        /** setters */
        void setPosition(const maths::Vector3f &position);
        void setRotation(const maths::Vector3f &rotation);
        void setScale(const maths::Vector3f &scale);

    protected:
        maths::Vector3f _position = {};
        maths::Vector3f _rotation = {};
        maths::Vector3f _scale = {1.0f, 1.0f, 1.0f};

        u32 _index_count = 0;

        VulkanBuffer _vertex_buffer;
        VulkanBuffer _index_buffer;
};

}// namespace vke
