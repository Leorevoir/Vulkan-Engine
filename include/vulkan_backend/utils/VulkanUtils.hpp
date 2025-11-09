#pragma once

#include <vulkan_backend/Backend.hpp>

#include <optional>
#include <string>
#include <vector>

namespace lumen {

struct QueueFamilyIndices {

        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const;
};

struct SwapChainSupportDetails {

        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
};

namespace utils {

QueueFamilyIndices find_queue_family(VkPhysicalDevice device, VkSurfaceKHR surface);
SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device, VkSurfaceKHR surface);
bool check_device_extension_support(VkPhysicalDevice device);

}// namespace utils

}// namespace lumen
