#include "vulkan_backend/core/Instance.hpp"
#include "vulkan_backend/core/Extensions.hpp"
#include "vulkan_backend/core/ValidationLayers.hpp"
#include "vulkan_backend/utils/Config.hpp"
#include "vulkan_backend/utils/Result.hpp"

// clang-format off

/**
 * static helpers
 */

namespace {

static VkResult __create_debug_utils_messenger(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger
) noexcept
{
    const auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}


static void __destroy_debug_utils_messenger(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator
) noexcept
{
    const auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

}// namespace
// clang-format on

/**
 * public
 */

lumen::Instance::Instance(const char *appName)
{
    _create_instance(appName);
    _setup_messenger_debuger();
}

lumen::Instance::~Instance() noexcept
{
    if (config::ENABLE_VALIDATION_LAYERS) {
        __destroy_debug_utils_messenger(_instance, _debugMessenger, nullptr);
    }
    vkDestroyInstance(_instance, nullptr);
}

/**
 * getters
 */

VkInstance lumen::Instance::handle() const noexcept
{
    return _instance;
}

/**
* private
*/

void lumen::Instance::_create_instance(const char *appName)
{
    if (config::ENABLE_VALIDATION_LAYERS && !ValidationLayers::checkSupport(config::VALIDATION_LAYERS)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lumen";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const auto extensions = get_required_instance_extensions(config::ENABLE_VALIDATION_LAYERS);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    if (config::ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(config::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = config::VALIDATION_LAYERS.data();
        ValidationLayers::populate(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    vk_check(vkCreateInstance(&createInfo, nullptr, &_instance), "failed to create instance!");
}

void lumen::Instance::_setup_messenger_debuger()
{
    if (!config::ENABLE_VALIDATION_LAYERS) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    ValidationLayers::populate(createInfo);

    vk_check(__create_debug_utils_messenger(_instance, &createInfo, nullptr, &_debugMessenger), "failed to set up debug messenger!");
}
