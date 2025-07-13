#pragma once

#include <VK/Error.hpp>

#include <VK/Backend.hpp>
#include <VK/Macros.hpp>

namespace vk {

namespace priv {

class VK_HIDDEN VulkanEngineBase
{
    public:
        VulkanEngineBase();
        virtual ~VulkanEngineBase() = default;

    protected:
        virtual void destroy();

        VkQueue _queue = VK_NULL_PTR;
        VkDevice _device = VK_NULL_PTR;
        VkCommandPool _commandPool = VK_NULL_PTR;

    private:
};

}// namespace priv

}// namespace vk
