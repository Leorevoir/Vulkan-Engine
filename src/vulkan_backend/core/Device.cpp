#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/commands/CommandQueue.hpp"
#include "vulkan_backend/core/Instance.hpp"
#include "vulkan_backend/surface/Surface.hpp"
#include "vulkan_backend/utils/Config.hpp"
#include "vulkan_backend/utils/Result.hpp"

#include <set>
#include <vector>

/**
 * public
 */

lumen::Device::Device(Instance &instance, Surface &surface)
{
    _pick_physical_device(instance, surface);
    _create_logical_device(surface);
}

lumen::Device::~Device() noexcept
{
    vkDestroyDevice(_device, nullptr);
}

/**
 * getters
 */

VkDevice lumen::Device::logicalDevice() const noexcept
{
    return _device;
}

VkPhysicalDevice lumen::Device::physicalDevice() const noexcept
{
    return _physicalDevice;
}

lumen::CommandQueue &lumen::Device::getGraphicsQueue() const noexcept
{
    return *_graphicsQueue;
}

lumen::CommandQueue &lumen::Device::getPresentQueue() const noexcept
{
    return *_presentQueue;
}

const lumen::QueueFamilyIndices &lumen::Device::getQueueFamilyIndices() const noexcept
{
    return _indices;
}

/**
* private
*/

void lumen::Device::_pick_physical_device(Instance &instance, Surface &surface)
{
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance.handle(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.handle(), &deviceCount, devices.data());

    for (const auto &device : devices) {

        if (_is_device_suitable(device, surface)) {
            _physicalDevice = device;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void lumen::Device::_create_logical_device(Surface &surface)
{
    _indices = utils::find_queue_family(_physicalDevice, surface.handle());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {_indices.graphicsFamily.value(), _indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(config::DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = config::DEVICE_EXTENSIONS.data();

    if (config::ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(config::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = config::VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    vk_check(vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device), "failed to create logical device!");

    _graphicsQueue = std::make_unique<CommandQueue>(_device, _indices.graphicsFamily.value(), 0);
    _presentQueue = std::make_unique<CommandQueue>(_device, _indices.presentFamily.value(), 0);
}

bool lumen::Device::_is_device_suitable(VkPhysicalDevice device, Surface &surface)
{
    const QueueFamilyIndices indices = utils::find_queue_family(device, surface.handle());
    const bool extensionsSupported = utils::check_device_extension_support(device);
    bool swapChainAdequate = false;

    if (extensionsSupported) {
        const SwapChainSupportDetails swapChainSupport = utils::query_swapchain_support(device, surface.handle());

        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
