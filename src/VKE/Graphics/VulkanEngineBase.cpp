#include <VKE/Graphics/VulkanEngineBase.hpp>
#include <VKE/Memory.hpp>

/**
* public
*/

void vke::priv::VulkanEngineBase::init_vulkan(Window &window)
{
    _create_vulkan_instance(window);
    _pick_physical_device();
    _create_logical_device();
}

/**
* protected
*/

void vke::priv::VulkanEngineBase::destroy()
{
    if (!_is_running) {
        return;
    }
    VKE_ASSERT(vkQueueWaitIdle(_queue));
    vkDeviceWaitIdle(_device);
    VKE_SAFE_CLEAN(_descriptorPool, vkDestroyDescriptorPool(_device, _descriptorPool, nullptr));
    VKE_SAFE_CLEAN(_renderPass, vkDestroyRenderPass(_device, _renderPass, nullptr));

    for (auto &shader : _shadersModules) {
        VKE_SAFE_CLEAN(shader, vkDestroyShaderModule(_device, shader, nullptr));
    }

    VKE_SAFE_CLEAN(_pipelineCache, vkDestroyPipelineCache(_device, _pipelineCache, nullptr));

    VKE_SAFE_CLEAN(_semaphores.present, vkDestroySemaphore(_device, _semaphores.present, nullptr));
    VKE_SAFE_CLEAN(_semaphores.render, vkDestroySemaphore(_device, _semaphores.render, nullptr));

    for (auto &fence : _waitFences) {
        VKE_SAFE_CLEAN(fence, vkDestroyFence(_device, fence, nullptr));
    }

    VKE_SAFE_CLEAN(_commandPool, vkDestroyCommandPool(_device, _commandPool, nullptr));

    memory::clean(_device);
    VKE_SAFE_CLEAN(_instance, vkDestroyInstance(_instance, nullptr));
    _is_running = false;
}

/**
* private
*/

void vke::priv::VulkanEngineBase::_create_vulkan_instance(vke::Window &window)
{
    const char *title = window.getTitle().c_str();
    VkApplicationInfo app_info = {};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = title;
    app_info.pEngineName = title;
    app_info.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> instance_extensions = {VK_KHR_SURFACE_EXTENSION_NAME};
#if defined(VKE_USE_PLATFORM_XCB_KHR)
    instance_extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

    if (_enabled_instance_extensions.size() > 0) {
        for (const auto &ext : _enabled_instance_extensions) {
            instance_extensions.push_back(ext);
        }
    }

    VkInstanceCreateInfo instance_create_info = {};

    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext = VKE_NULL_PTR;
    instance_create_info.pApplicationInfo = &app_info;

    if (instance_extensions.size() > 0) {
        instance_create_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
        instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    }

    VKE_ASSERT(vkCreateInstance(&instance_create_info, VKE_NULL_PTR, &_instance));
}

void vke::priv::VulkanEngineBase::_pick_physical_device()
{
    u32 gpu_count = 0;

    VKE_ASSERT(vkEnumeratePhysicalDevices(_instance, &gpu_count, VKE_NULL_PTR));
    assert(gpu_count > 0 && "No Vulkan compatible GPU found");

    std::vector<VkPhysicalDevice> physical_devices(gpu_count);
    VKE_ASSERT(vkEnumeratePhysicalDevices(_instance, &gpu_count, physical_devices.data()));

    if (_selectGPUIndex <= gpu_count) {
        _physicalDevice = physical_devices[_selectGPUIndex];
    }

    vkGetPhysicalDeviceProperties(_physicalDevice, &_deviceProperties);
    vkGetPhysicalDeviceFeatures(_physicalDevice, &_deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_deviceMemoryProperties);
}

void vke::priv::VulkanEngineBase::_destroy_surface()
{
    _swapChain.destroy();
}
