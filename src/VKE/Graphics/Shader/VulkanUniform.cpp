#include <VKE/Graphics/Shader/VulkanUniform.hpp>

/**
* public
*/

vke::VulkanBuffer &vke::VulkanUniform::getUniformBuffer()
{
    return _uniform_buffer;
}
