#include <VKE/Graphics/VulkanObject.hpp>

#include <iostream>

/**
* public
*/

void vke::VulkanObject::setContext(VulkanContext *context)
{
    _context = context;
}
