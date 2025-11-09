#pragma once

#include <vulkan_backend/Backend.hpp>

#include <vector>

// clang-format off

namespace lumen {

class ValidationLayers
{
    public:
        static bool checkSupport(const std::vector<const char *> &layers) noexcept;

        static void populate(VkDebugUtilsMessengerCreateInfoEXT &createInfo) noexcept;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData
        ) noexcept;
};

}// namespace lumen
