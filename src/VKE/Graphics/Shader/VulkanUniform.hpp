#pragma once

#include <VKE/Graphics/Device/VulkanBuffer.hpp>
#include <VKE/Macros.hpp>

namespace vke {

class VKE_API VulkanUniform
{
    public:
        virtual ~VulkanUniform() = default;

        vke::VulkanBuffer &getUniformBuffer();

    protected:
        vke::VulkanBuffer _uniform_buffer;
};

}// namespace vke
