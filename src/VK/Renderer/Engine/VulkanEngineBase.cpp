#include <VK/Renderer/Engine/VulkanEngineBase.hpp>

/**
* public
*/

vk::priv::VulkanEngineBase::VulkanEngineBase()
{
    /* __ctor__ */
}

/**
* protected
*/

void vk::priv::VulkanEngineBase::destroy()
{
    VK_ASSERT(vkQueueWaitIdle(_queue));
}

/**
* private
*/
