#pragma once

#include <VKE/Graphics/Device/VulkanBuffer.hpp>
#include <VKE/Macros.hpp>

namespace vke {

namespace priv {

class VKE_API VulkanUniformBase
{
    public:
        virtual ~VulkanUniformBase() = default;

        vke::VulkanBuffer &getBuffer();

    protected:
        vke::VulkanBuffer _buffer;
};

}// namespace priv

}// namespace vke
