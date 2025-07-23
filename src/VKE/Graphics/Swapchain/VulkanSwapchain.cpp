#include <VKE/Error.hpp>
#include <VKE/Graphics/Swapchain/VulkanSwapchain.hpp>

/**
 * helper
 */

/**
 * following macros are inspired by the LearnVulkan tutorial
 * they were made by Sascha Willems I just adapted them to my needs
 */

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                                                                                                                         \
    {                                                                                                                                                                    \
        fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk" #entrypoint));                                                            \
        if (fp##entrypoint == VKE_NULLPTR) {                                                                                                                             \
            throw exception::RuntimeError("vke::priv::Swapchain", "Failed to get instance procedure address for vk" #entrypoint);                                        \
        }                                                                                                                                                                \
    }

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                                                                                                            \
    {                                                                                                                                                                    \
        fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk" #entrypoint));                                                               \
        if (fp##entrypoint == VKE_NULLPTR) {                                                                                                                             \
            throw exception::RuntimeError("vke::priv::Swapchain", "Failed to get device procedure address for vk" #entrypoint);                                          \
        }                                                                                                                                                                \
    }

/**
* public
*/

void vke::priv::VulkanSwapchain::init(std::shared_ptr<Window> &window)
{
    u32 queue_count = 0;
    u32 graphics_queue_node_index = UINT32_MAX;
    u32 presenting_queue_node_index = UINT32_MAX;

    window->createVulkanSurface(_instance, _surface);

    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, VKE_NULLPTR);
    assert(queue_count >= 1);

    std::vector<VkQueueFamilyProperties> queue_properties(queue_count);
    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, queue_properties.data());

    /**
     * @brief checks which queue families of the physical device can present images to the surface.
     */

    std::vector<u32> support_presenting(queue_count);
    for (u32 i = 0; i < queue_count; i++) {
        fpGetPhysicalDeviceSurfaceSupportKHR(_physical_device, i, _surface, &support_presenting[i]);
    }

    /**
     * search for a graphics queue & a presenting queue in the queue families
     */

    for (u32 i = 0; i < queue_count; i++) {

        if ((queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {

            if (graphics_queue_node_index == UINT32_MAX) {
                graphics_queue_node_index = i;
            }

            if (support_presenting[i]) {
                graphics_queue_node_index = i;
                presenting_queue_node_index = i;
                break;
            }
        }
    }
    if (presenting_queue_node_index == UINT32_MAX) {

        for (u32 i = 0; i < queue_count; ++i) {

            if (support_presenting[i]) {
                presenting_queue_node_index = i;
                break;
            }
        }
    }

    /** 
    * otherwise it is an error :(
    */

    if (graphics_queue_node_index == UINT32_MAX || presenting_queue_node_index == UINT32_MAX) {
        throw vke::exception::RuntimeError("vke::priv::VulkanSwapchain::init", "Could not find a graphics and/or presenting queue!");
    }

    if (graphics_queue_node_index != presenting_queue_node_index) {
        throw vke::exception::RuntimeError("vke::priv::VulkanSwapchain::init", "Separate graphics and presenting queues are not supported yet!");
    }

    _queue_index = graphics_queue_node_index;

    u32 format_count;

    VKE_ASSERT(fpGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, VKE_NULLPTR));
    assert(format_count > 0);

    std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
    VKE_ASSERT(fpGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, surface_formats.data()));

    if (format_count == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED) {
        _color._format = VK_FORMAT_B8G8R8A8_UNORM;
        _color._space = surface_formats[0].colorSpace;
        return;
    }

    /** 
     * iterate over the list of available surface formats 
     * check for the presence of VK_FORMAT_B8G8R8A8_UNORM
     * otherwise select the first available format
     */

    for (auto &&surface : surface_formats) {
        if (surface.format == VK_FORMAT_B8G8R8A8_UNORM) {
            _color._format = surface.format;
            _color._space = surface.colorSpace;
            return;
        }
    }

    _color._format = surface_formats[0].format;
    _color._space = surface_formats[0].colorSpace;
}

void vke::priv::VulkanSwapchain::connect(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device)
{
    _instance = instance;
    _physical_device = physical_device;
    _device = device;

    GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceSupportKHR);
    GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
    GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
    GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
    GET_DEVICE_PROC_ADDR(device, CreateSwapchainKHR);
    GET_DEVICE_PROC_ADDR(device, DestroySwapchainKHR);
    GET_DEVICE_PROC_ADDR(device, GetSwapchainImagesKHR);
    GET_DEVICE_PROC_ADDR(device, AcquireNextImageKHR);
    GET_DEVICE_PROC_ADDR(device, QueuePresentKHR);
}

/**
 * create the swapchain and get its images with given size
 */

void vke::priv::VulkanSwapchain::create(vke::maths::Vector2u &size, bool vsync)
{
    u32 width = size.x;
    u32 height = size.y;

    create(&width, &height, vsync);
    size.x = width;
    size.y = height;
}

void vke::priv::VulkanSwapchain::create(u32 *width, u32 *height, bool vsync)
{
    VkSwapchainKHR old_swapchain = _swapchain;
    VkSurfaceCapabilitiesKHR surface_capabilities;

    VKE_ASSERT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &surface_capabilities));

    u32 presenting_mode_count;
    VKE_ASSERT(fpGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &presenting_mode_count, VKE_NULLPTR));
    assert(presenting_mode_count > 0);

    std::vector<VkPresentModeKHR> presenting_modes(presenting_mode_count);
    VKE_ASSERT(fpGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &presenting_mode_count, presenting_modes.data()));

    VkExtent2D extent = {};

    /**
     * @brief if == 0xFFFFFFFF, the size of the surface will be set by the swapchain
     */

    if (surface_capabilities.currentExtent.width == (u32) -1) {
        extent.width = *width;
        extent.height = *height;
    } else {
        extent = surface_capabilities.currentExtent;
        *width = surface_capabilities.currentExtent.width;
        *height = surface_capabilities.currentExtent.height;
    }

    VkPresentModeKHR swapchain_presenting_mode = VK_PRESENT_MODE_FIFO_KHR;

    if (!vsync) {

        for (size_t i = 0; i < presenting_mode_count; i++) {

            if (presenting_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                swapchain_presenting_mode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }

            if ((swapchain_presenting_mode != VK_PRESENT_MODE_MAILBOX_KHR) && (presenting_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
                swapchain_presenting_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    u32 desired_number_of_swapchain_images = surface_capabilities.minImageCount + 1;
    if ((surface_capabilities.maxImageCount > 0) && (desired_number_of_swapchain_images > surface_capabilities.maxImageCount)) {
        desired_number_of_swapchain_images = surface_capabilities.maxImageCount;
    }

    VkSurfaceTransformFlagsKHR surface_pre_transform_flags;
    if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        surface_pre_transform_flags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        surface_pre_transform_flags = surface_capabilities.currentTransform;
    }

    /**
     * find a supported composite alpha format
     */

    VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    const VkCompositeAlphaFlagBitsKHR composite_alpha_flags[] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };

    for (auto &composite_alpha_flag : composite_alpha_flags) {

        if (surface_capabilities.supportedCompositeAlpha & static_cast<VkCompositeAlphaFlagsKHR>(composite_alpha_flag)) {

            composite_alpha = composite_alpha_flag;
            break;
        };
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext = VKE_NULLPTR;
    swapchain_create_info.surface = _surface;
    swapchain_create_info.minImageCount = desired_number_of_swapchain_images;
    swapchain_create_info.imageFormat = _color._format;
    swapchain_create_info.imageColorSpace = _color._space;
    swapchain_create_info.imageExtent = {extent.width, extent.height};
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.preTransform = (VkSurfaceTransformFlagBitsKHR) surface_pre_transform_flags;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = VKE_NULLPTR;
    swapchain_create_info.presentMode = swapchain_presenting_mode;
    swapchain_create_info.oldSwapchain = old_swapchain;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.compositeAlpha = composite_alpha;

    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
        swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VKE_ASSERT(vkCreateSwapchainKHR(_device, &swapchain_create_info, nullptr, &_swapchain));

    for (u32 i = 0; i < _buffers.size(); i++) {
        if (_buffers[i]._view) {
            vkDestroyImageView(_device, _buffers[i]._view, nullptr);
            _buffers[i]._view = VKE_NULLPTR;
        }
    }
    _buffers.clear();
    if (old_swapchain != VKE_NULLPTR) {
        fpDestroySwapchainKHR(_device, old_swapchain, nullptr);
        old_swapchain = VKE_NULLPTR;
    }
    VKE_ASSERT(fpGetSwapchainImagesKHR(_device, _swapchain, &_image_count, VKE_NULLPTR));

    /**
     * get the swapchain images
     */

    _images.resize(_image_count);
    VKE_ASSERT(fpGetSwapchainImagesKHR(_device, _swapchain, &_image_count, _images.data()));

    /** 
    * get the swapchain buffer containing the image and imageview 
    */

    _buffers.resize(_image_count);
    for (u32 i = 0; i < _image_count; i++) {

        VkImageViewCreateInfo image_view_create_info = {};

        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = VKE_NULLPTR;
        image_view_create_info.format = _color._format;
        image_view_create_info.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.flags = 0;

        _buffers[i]._image = _images[i];

        image_view_create_info.image = _buffers[i]._image;

        VKE_ASSERT(vkCreateImageView(_device, &image_view_create_info, nullptr, &_buffers[i]._view));
    }
}

/**
 * get the next image in the swapchain
 */

VkResult vke::priv::VulkanSwapchain::next(VkSemaphore present_semaphore, u32 *image_index)
{
    if (_swapchain) {
        return fpAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, present_semaphore, static_cast<VkFence>(VKE_NULLPTR), image_index);
    }

    return VK_SUCCESS;
}

/**
 * queue (for presentation)
 */

VkResult vke::priv::VulkanSwapchain::queue(VkQueue queue, u32 image_index, VkSemaphore wait_semaphore)
{
    VkPresentInfoKHR presentation_info = {};
    presentation_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentation_info.pNext = VKE_NULLPTR;
    presentation_info.swapchainCount = 1;
    presentation_info.pSwapchains = &_swapchain;
    presentation_info.pImageIndices = &image_index;

    if (wait_semaphore) {
        presentation_info.pWaitSemaphores = &wait_semaphore;
        presentation_info.waitSemaphoreCount = 1;
    }

    if (_swapchain) {
        return vkQueuePresentKHR(queue, &presentation_info);
    }

    return VK_SUCCESS;
}

void vke::priv::VulkanSwapchain::destroy()
{
    for (u32 i = 0; i < _buffers.size(); i++) {
        if (_buffers[i]._view) {
            vkDestroyImageView(_device, _buffers[i]._view, nullptr);
        }
        if (_buffers[i]._image) {
            vkDestroyImage(_device, _buffers[i]._image, nullptr);
        }
        _buffers[i]._view = VKE_NULLPTR;
        _buffers[i]._image = VKE_NULLPTR;
    }
}
