#include <VKE/Graphics/Shader/Base/VulkanUniformBase.hpp>

/**
* public
*/

vke::VulkanBuffer &vke::priv::VulkanUniformBase::getBuffer()
{
    return _buffer;
}
