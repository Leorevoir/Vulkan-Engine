#include <VKE/Graphics/Context/VulkanContext.hpp>

/**
 * public
 */

vke::VulkanContext::VulkanContext(priv::VulkanDevice *device, maths::Vector2u *screen_size) : _device(device), _screen_size(screen_size)
{
    /* __ctor__ */
}
