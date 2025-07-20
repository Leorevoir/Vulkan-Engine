#include <VKE/Error.hpp>
#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Memory.hpp>

/**
* public
*/

vke::priv::VulkanDevice::VulkanDevice(VkPhysicalDevice physical_device)
{
    assert(physical_device != VKE_NULLPTR);
    _physicalDevice = physical_device;
    vkGetPhysicalDeviceProperties(_physicalDevice, &_properties);
    vkGetPhysicalDeviceFeatures(_physicalDevice, &_features);
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_memoryProperties);

    uint32_t queue_family_count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queue_family_count, VKE_NULLPTR);
    assert(queue_family_count > 0);
    _queueFamilyProperties.resize(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queue_family_count, _queueFamilyProperties.data());

    u32 extension_count = 0;

    vkEnumerateDeviceExtensionProperties(_physicalDevice, VKE_NULLPTR, &extension_count, VKE_NULLPTR);
    if (extension_count > 0) {
        std::vector<VkExtensionProperties> extensions(extension_count);

        if (vkEnumerateDeviceExtensionProperties(_physicalDevice, VKE_NULLPTR, &extension_count, extensions.data()) == VK_SUCCESS) {

            for (const auto &ext : extensions) {
                _supportedExtensions.push_back(ext.extensionName);
            }
        }
    }
}

vke::priv::VulkanDevice::~VulkanDevice()
{
    if (_commandPool) {
        vkDestroyCommandPool(_logicalDevice, _commandPool, VKE_NULLPTR);
        _commandPool = VKE_NULLPTR;
    }
    if (_logicalDevice) {
        vkDestroyDevice(_logicalDevice, VKE_NULLPTR);
        _logicalDevice = VKE_NULLPTR;
    }
}

u32 vke::priv::VulkanDevice::getMemoryType(u32 type_bits, VkMemoryPropertyFlags properties, u32 *memory_type_index) const
{
    for (u32 i = 0; i < _memoryProperties.memoryTypeCount; i++) {

        if ((type_bits & 1) == 1) {

            if ((_memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {

                if (memory_type_index) {
                    *memory_type_index = true;
                }

                return i;
            }
        }
        type_bits >>= 1;
    }

    if (memory_type_index) {
        *memory_type_index = false;
        return 0;
    }

    throw exception::RuntimeError("vke::priv::VulkanDevice::getMemoryType", "Failed to find suitable memory type.");
}

u32 vke::priv::VulkanDevice::getQueueFamilyIndex(VkQueueFlagBits queue_flags) const
{
    if (static_cast<u32>(queue_flags) & (VK_QUEUE_COMPUTE_BIT)) {
        for (u32 i = 0; i < static_cast<u32>(_queueFamilyProperties.size()); ++i) {
            if ((_queueFamilyProperties[i].queueFlags & static_cast<u32>(queue_flags)) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                return i;
            }
        }
    }

    if (queue_flags & VK_QUEUE_TRANSFER_BIT) {

        for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++) {
            if ((_queueFamilyProperties[i].queueFlags & static_cast<u32>(queue_flags)) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
                && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
                return i;
            }
        }
    }

    for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++) {
        if (_queueFamilyProperties[i].queueFlags & static_cast<u32>(queue_flags)) {
            return i;
            break;
        }
    }

    throw exception::RuntimeError("vke::priv::VulkanDevice::getQueueFamilyIndex", "Failed to find suitable queue family index for requested queue flags.");
}

VkResult vke::priv::VulkanDevice::createLogicalDevice(VkPhysicalDeviceFeatures enabled_features, std::vector<const char *> enabled_extensions, void *next_chain,
    bool use_swapchain, VkQueueFlags requested_queue_types)
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
    const f32 default_queue_priority(0.0f);

    /** @brief graphics queue */
    if (requested_queue_types & VK_QUEUE_GRAPHICS_BIT) {
        _queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = _queueFamilyIndices.graphics;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &default_queue_priority;
        queueCreateInfos.push_back(queue_info);
    } else {
        _queueFamilyIndices.graphics = 0;
    }

    /** @brief compute queue */
    if (requested_queue_types & VK_QUEUE_COMPUTE_BIT) {
        _queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
        if (_queueFamilyIndices.compute != _queueFamilyIndices.graphics) {
            VkDeviceQueueCreateInfo queue_info{};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = _queueFamilyIndices.compute;
            queue_info.queueCount = 1;
            queue_info.pQueuePriorities = &default_queue_priority;
            queueCreateInfos.push_back(queue_info);
        }
    } else {
        _queueFamilyIndices.compute = _queueFamilyIndices.graphics;
    }

    /** @brief transfer queue */
    if (requested_queue_types & VK_QUEUE_TRANSFER_BIT) {
        _queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
        if ((_queueFamilyIndices.transfer != _queueFamilyIndices.graphics) && (_queueFamilyIndices.transfer != _queueFamilyIndices.compute)) {
            VkDeviceQueueCreateInfo queue_info{};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = _queueFamilyIndices.transfer;
            queue_info.queueCount = 1;
            queue_info.pQueuePriorities = &default_queue_priority;
            queueCreateInfos.push_back(queue_info);
        }
    } else {
        _queueFamilyIndices.transfer = _queueFamilyIndices.graphics;
    }

    std::vector<const char *> deviceExtensions(enabled_extensions);
    if (use_swapchain) {
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    ;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &enabled_features;

    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
    if (next_chain) {
        physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        physicalDeviceFeatures2.features = enabled_features;
        physicalDeviceFeatures2.pNext = next_chain;
        deviceCreateInfo.pEnabledFeatures = VKE_NULLPTR;
        deviceCreateInfo.pNext = &physicalDeviceFeatures2;
    }

    if (isExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) {
        deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
        _enableDebugMarkers = true;
    }

    if (deviceExtensions.size() > 0) {
        deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    const VkResult result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, VKE_NULLPTR, &_logicalDevice);

    if (result == VK_SUCCESS) {
        _commandPool = createCommandPool(_queueFamilyIndices.graphics);
    }

    _enabledFeatures = enabled_features;

    return result;
}

VkResult vke::priv::VulkanDevice::createBuffer(VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VkDeviceSize size, VkBuffer *buffer,
    VkDeviceMemory *memory, void *data)
{
    VkBufferCreateInfo buffer_create_info = helper::create_buffer_info(usage_flags, size);

    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VKE_ASSERT(vkCreateBuffer(_logicalDevice, &buffer_create_info, VKE_NULLPTR, buffer));

    VkMemoryRequirements mem_reqs;
    VkMemoryAllocateInfo mem_alloc = {};

    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    vkGetBufferMemoryRequirements(_logicalDevice, *buffer, &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;
    mem_alloc.memoryTypeIndex = getMemoryType(mem_reqs.memoryTypeBits, memory_property_flags);
    VKE_ASSERT(vkAllocateMemory(_logicalDevice, &mem_alloc, VKE_NULLPTR, memory));

    if (data != VKE_NULLPTR) {
        void *mapped;
        VKE_ASSERT(vkMapMemory(_logicalDevice, *memory, 0, size, 0, &mapped));
        memory::copy(mapped, data, size);

        if ((memory_property_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0) {
            VkMappedMemoryRange range = {};

            range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            range.memory = *memory;
            range.offset = 0;
            range.size = size;
            vkFlushMappedMemoryRanges(_logicalDevice, 1, &range);
        }
        vkUnmapMemory(_logicalDevice, *memory);
    }

    VKE_ASSERT(vkBindBufferMemory(_logicalDevice, *buffer, *memory, 0));

    return VK_SUCCESS;
}

VkResult vke::priv::VulkanDevice::createBuffer(VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VulkanBuffer *buffer, VkDeviceSize size,
    void *data)
{
    buffer->_device = _logicalDevice;

    VkBufferCreateInfo buffer_create_info = helper::create_buffer_info(usage_flags, size);

    VKE_ASSERT(vkCreateBuffer(_logicalDevice, &buffer_create_info, VKE_NULLPTR, &buffer->_buffer));

    VkMemoryRequirements mem_reqs;
    VkMemoryAllocateInfo mem_alloc = {};

    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    vkGetBufferMemoryRequirements(_logicalDevice, buffer->_buffer, &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;
    mem_alloc.memoryTypeIndex = getMemoryType(mem_reqs.memoryTypeBits, memory_property_flags);
    VKE_ASSERT(vkAllocateMemory(_logicalDevice, &mem_alloc, VKE_NULLPTR, &buffer->_memory));

    buffer->_alignment = mem_reqs.alignment;
    buffer->_size = size;
    buffer->_usageFlags = usage_flags;
    buffer->_memoryPropertyFlags = memory_property_flags;

    if (data != VKE_NULLPTR) {
        VKE_ASSERT(buffer->map());
        memory::copy(buffer->_mapped, data, size);
        if ((memory_property_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
            buffer->flush();

        buffer->unmap();
    }

    buffer->setupDescriptorInfo();
    return buffer->bind();
}

void vke::priv::VulkanDevice::copyBuffer(VulkanBuffer *dst, VulkanBuffer *src, VkQueue queue, VkBufferCopy *copy_region)
{
    assert(dst->_size <= src->_size);
    assert(src->_buffer);

    VkCommandBuffer cmd = createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
    VkBufferCopy buffer_copy{};

    if (copy_region) {
        buffer_copy = *copy_region;
    } else {
        buffer_copy.size = src->_size;
    }

    vkCmdCopyBuffer(cmd, src->_buffer, dst->_buffer, 1, &buffer_copy);
    flushCommandBuffer(cmd, queue);
}

VkCommandPool vke::priv::VulkanDevice::createCommandPool(u32 queue_family_index, VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo pool_info{};

    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = queue_family_index;
    pool_info.flags = flags;

    VkCommandPool command_pool = VKE_NULLPTR;

    VKE_ASSERT(vkCreateCommandPool(_logicalDevice, &pool_info, VKE_NULLPTR, &command_pool));
    return command_pool;
}

VkCommandBuffer vke::priv::VulkanDevice::createCommandBuffer(VkCommandBufferLevel level, VkCommandPool command_pool, bool begin)
{
    VkCommandBufferAllocateInfo cmd_buff_alloc_info = helper::allocate_command_buffer(command_pool, level, 1);
    VkCommandBuffer cmd;

    VKE_ASSERT(vkAllocateCommandBuffers(_logicalDevice, &cmd_buff_alloc_info, &cmd));
    if (begin) {
        VkCommandBufferBeginInfo cmd_begin_info{};
        cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VKE_ASSERT(vkBeginCommandBuffer(cmd, &cmd_begin_info));
    }
    return cmd;
}

VkCommandBuffer vke::priv::VulkanDevice::createCommandBuffer(VkCommandBufferLevel level, bool begin)
{
    return createCommandBuffer(level, _commandPool, begin);
}

void vke::priv::VulkanDevice::flushCommandBuffer(VkCommandBuffer command_buffer, VkQueue queue, VkCommandPool pool, bool free)
{
    if (command_buffer == VKE_NULLPTR) {
        return;
    }

    VKE_ASSERT(vkEndCommandBuffer(command_buffer));

    VkSubmitInfo submit_info{};

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    VkFenceCreateInfo fence_info{};

    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = 0;

    VkFence fence;

    VKE_ASSERT(vkCreateFence(_logicalDevice, &fence_info, VKE_NULLPTR, &fence));
    VKE_ASSERT(vkQueueSubmit(queue, 1, &submit_info, fence));
    VKE_ASSERT(vkWaitForFences(_logicalDevice, 1, &fence, VK_TRUE, VKE_FENCE_TIMEOUT));
    vkDestroyFence(_logicalDevice, fence, VKE_NULLPTR);
    if (free) {
        vkFreeCommandBuffers(_logicalDevice, pool, 1, &command_buffer);
    }
}

void vke::priv::VulkanDevice::flushCommandBuffer(VkCommandBuffer command_buffer, VkQueue queue, bool free)
{
    flushCommandBuffer(command_buffer, queue, _commandPool, free);
}

bool vke::priv::VulkanDevice::isExtensionSupported(std::string extension) const
{
    return std::find(_supportedExtensions.begin(), _supportedExtensions.end(), extension) != _supportedExtensions.end();
}

VkFormat vke::priv::VulkanDevice::getSupportedDepthFormat(bool check_sampling_support)
{
    const std::vector<VkFormat> depth_formats = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM};
    for (const auto &format : depth_formats) {
        VkFormatProperties format_properties;
        vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &format_properties);
        if (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            if (check_sampling_support) {
                if (!(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)) {
                    continue;
                }
            }
            return format;
        }
    }
    throw exception::RuntimeError("vke::priv::VulkanDevice::getSupportedDepthFormat",
        "Failed to find a suitable depth format. Ensure that the physical device supports depth formats.");
}

vke::priv::VulkanDevice::operator VkDevice()
{
    return _logicalDevice;
}
