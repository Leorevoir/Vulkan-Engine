#include "vulkan_backend/core/Extensions.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

/**
* public
*/

const std::vector<const char *> lumen::get_required_instance_extensions(const bool enableValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
