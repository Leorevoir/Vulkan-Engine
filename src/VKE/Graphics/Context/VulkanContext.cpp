#include <VKE/Graphics/Context/VulkanContext.hpp>

/**
 * public
 */

vke::VulkanContext::VulkanContext(priv::VulkanDevice *device, maths::Vector2u *screen_size) : _device(device), _screen_size(screen_size)
{
    /* __ctor__ */
}

/**
 * setters
 */

void vke::VulkanContext::setPipelines(priv::VulkanPipelines *pipelines)
{
    _pipelines = pipelines;
}

void vke::VulkanContext::setRenderPass(VkRenderPass *render_pass)
{
    _render_pass = render_pass;
}

void vke::VulkanContext::setDescriptorSet(priv::VulkanDescriptorSet *descriptor_set)
{
    _descriptor_set = descriptor_set;
}

/**
 * getters
*/

VkDevice &vke::VulkanContext::getLogicalDevice()
{
    return _device->_logicalDevice;
}

VkPhysicalDevice &vke::VulkanContext::getPhysicalDevice()
{
    return _device->_physicalDevice;
}

VkRenderPass *vke::VulkanContext::getRenderPass()
{
    return _render_pass;
}

vke::priv::VulkanDevice *vke::VulkanContext::getDevice()
{
    return _device;
}

vke::priv::VulkanPipelines *vke::VulkanContext::getPipelines()
{
    return _pipelines;
}

vke::maths::Vector2u *vke::VulkanContext::getScreenSize()
{
    return _screen_size;
}

vke::priv::VulkanDescriptorSet *vke::VulkanContext::getDescriptorSet()
{
    return _descriptor_set;
}
