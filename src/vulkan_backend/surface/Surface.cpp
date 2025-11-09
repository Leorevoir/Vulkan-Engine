#include <vulkan_backend/core/Instance.hpp>
#include <vulkan_backend/surface/Surface.hpp>
#include <vulkan_backend/utils/Result.hpp>

/**
* public
*/

lumen::Surface::Surface(Instance &instance, GLFWwindow *window) : _instance(instance)
{
    vk_check(glfwCreateWindowSurface(instance.handle(), window, nullptr, &_surface), "failed to create window surface!");
}

lumen::Surface::~Surface() noexcept
{
    vkDestroySurfaceKHR(_instance.handle(), _surface, nullptr);
}

/**
* getters
*/

VkSurfaceKHR lumen::Surface::handle() const noexcept
{
    return _surface;
}
