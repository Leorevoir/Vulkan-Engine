#include <VKE/Error.hpp>
#include <VKE/Graphics/Engine/VulkanEngineBase.hpp>

#include <chrono>
#include <iostream>
#include <thread>

/**
* public
*/

vke::priv::VulkanEngineBase::VulkanEngineBase()
{
    _create_window();
    _create_vulkan_instance();
    _get_physical_device();
}

vke::priv::VulkanEngineBase::~VulkanEngineBase()
{
    _destroy();
}

void vke::priv::VulkanEngineBase::start()
{
    _swapchain.init(_window);
    _create_command_pool();
    _swapchain.create(_size, false);
    _create_command_buffer();
    _create_synchronization_objects();
    _create_depth_stencil();
    _create_render_pass();
    _create_pipeline_cache();
    _create_framebuffer();
    _descriptor_set = std::make_unique<VulkanDescriptorSet>(_device);
    _vertex_descriptor = std::make_unique<VulkanVertexDescriptor>();
    _pipelines = std::make_unique<VulkanPipelines>(_device, _vertex_descriptor->getState(), _pipeline_cache);
    _context = std::make_shared<VulkanContext>(_vulkan_device.get(), &_size);
    _context->_cmd_pool = _command_pool;
    _context->_pipeline_layout = &_pipeline_layout;
    _context->_pipeline_cache = _pipeline_cache;
    _context->_render_pass = _render_pass;
    _context->_queue = _queue;
}

/**
* protected
*/

void vke::priv::VulkanEngineBase::renderFrame()
{
    if (_paused || !_prepared) {
        return;
    }

    const auto time_start = std::chrono::high_resolution_clock::now();

    update();
    draw();
    render();

    const auto time_end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();

    _frame_time = static_cast<f32>(duration) / 1000.0f;
    vkDeviceWaitIdle(_device);
}

void vke::priv::VulkanEngineBase::render()
{
    if (_paused) {
        return;
    }

    _signal_frame = false;
    _acquire_frame();
    _submit_info.commandBufferCount = 1;
    _submit_info.pCommandBuffers = &_command_buffer[_current_buffer];

    if (_prepared) {
        VKE_ASSERT(vkQueueSubmit(_queue, 1, &_submit_info, VKE_NULLPTR));
    }

    _submit_frame();
    _signal_frame = true;
}

void vke::priv::VulkanEngineBase::waitForCurrentFrame()
{
    _paused = true;
    while (!_signal_frame) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void vke::priv::VulkanEngineBase::build_command_buffer()
{
    VkCommandBufferBeginInfo cmd_buffer_begin_info = {};
    cmd_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    for (u32 i = 0; i < _command_buffer.size(); ++i) {
        VKE_ASSERT(vkBeginCommandBuffer(_command_buffer[i], &cmd_buffer_begin_info));

        buildCommandBufferBeforeRenderPass();

        {
            VkClearValue clear_values[2] = {};
            clear_values[0].color = {{.1f, .2f, .3f, .1f}};
            clear_values[1].depthStencil = {1.0f, 0};
            VkRenderPassBeginInfo render_pass_begin_info = {};
            render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            render_pass_begin_info.renderPass = _render_pass;
            render_pass_begin_info.renderArea = {
                .offset = {.x = 0, .y = 0},
                .extent = {.width = _size.width, .height = _size.height},
            };
            render_pass_begin_info.clearValueCount = 2;
            render_pass_begin_info.pClearValues = clear_values;
            render_pass_begin_info.framebuffer = _framebuffers[i];

            vkCmdBeginRenderPass(_command_buffer[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindDescriptorSets(_command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_layout, 0, 1, &(_descriptor_set->get(0)), 0, VKE_NULLPTR);

            _set_viewports(_command_buffer[i]);
            drawObjects(_command_buffer[i]);

            vkCmdEndRenderPass(_command_buffer[i]);
        }

        buildCommandBufferAfterRenderPass();
        VKE_ASSERT(vkEndCommandBuffer(_command_buffer[i]));
    }
    vkQueueWaitIdle(_queue);
}

/**
* private
*/

/**
* initialization
*/

void vke::priv::VulkanEngineBase::_create_window()
{
    _window = std::make_shared<vke::Window>(_size, "Vulkan Engine");
}

void vke::priv::VulkanEngineBase::_create_vulkan_instance()
{
    VkApplicationInfo app_info = {};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan Engine";
    app_info.pEngineName = "Vulkan Engine";
    app_info.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> instance_extensions = {VK_KHR_SURFACE_EXTENSION_NAME};

    instance_extensions.push_back(_window->getVulkanExtension());

    for (const auto &ext : _enabled_extensions) {
        instance_extensions.push_back(ext);
    }

    std::vector<const char *> instance_layers;
#ifdef DEBUG
    instance_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
    instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    instance_create_info.enabledLayerCount = static_cast<uint32_t>(instance_layers.size());
    instance_create_info.ppEnabledLayerNames = instance_layers.data();

    VKE_ASSERT(vkCreateInstance(&instance_create_info, nullptr, &_instance));
}

void vke::priv::VulkanEngineBase::_get_physical_device()
{
    u32 gpu_count = 0;

    VKE_ASSERT(vkEnumeratePhysicalDevices(_instance, &gpu_count, VKE_NULLPTR));
    assert(gpu_count > 0 && "No Vulkan physical devices found");

    std::vector<VkPhysicalDevice> physical_devices(gpu_count);
    VKE_ASSERT(vkEnumeratePhysicalDevices(_instance, &gpu_count, physical_devices.data()));

    if (_selected_gpu_index <= gpu_count) {
        _physical_device = physical_devices[_selected_gpu_index];
    }

    vkGetPhysicalDeviceProperties(_physical_device, &_physical_device_properties);
    vkGetPhysicalDeviceFeatures(_physical_device, &_physical_device_features);
    vkGetPhysicalDeviceMemoryProperties(_physical_device, &_physical_device_memory_properties);

    if (_physical_device_features.multiViewport) {
        _enabled_features.multiViewport = VK_TRUE;
    }
    if (_physical_device_features.sampleRateShading) {
        _enabled_features.sampleRateShading = VK_TRUE;
    }
    _enabled_features.fillModeNonSolid = _physical_device_features.fillModeNonSolid ? VK_TRUE : VK_FALSE;

    getDeviceFeatures();

    _vulkan_device = std::make_unique<VulkanDevice>(_physical_device);
    _vulkan_device->createLogicalDevice(_enabled_features, _enabled_device_extensions, _device_create_next_chain);
    _device = _vulkan_device->_logicalDevice;

    vkGetDeviceQueue(_device, _vulkan_device->_queueFamilyIndices.graphics, 0, &_queue);

    const u32 valid_format = getSuportedDepthFormat(_physical_device, &_depth_format);
    assert(valid_format);

    _swapchain.connect(_instance, _physical_device, _device);

    VkSemaphoreCreateInfo semaphore_create_info = {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VKE_ASSERT(vkCreateSemaphore(_device, &semaphore_create_info, VKE_NULLPTR, &_semaphores._presentation));
    VKE_ASSERT(vkCreateSemaphore(_device, &semaphore_create_info, VKE_NULLPTR, &_semaphores._rendering));

    _submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    _submit_info.pWaitDstStageMask = &_submit_stages;
    _submit_info.waitSemaphoreCount = 1;
    _submit_info.pWaitSemaphores = &_semaphores._presentation;
    _submit_info.signalSemaphoreCount = 1;
    _submit_info.pSignalSemaphores = &_semaphores._rendering;
}

/**
* start
*/

void vke::priv::VulkanEngineBase::_create_command_pool()
{
    VkCommandPoolCreateInfo command_pool_create_info = {};

    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = _swapchain.getQueueIndex();
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VKE_ASSERT(vkCreateCommandPool(_device, &command_pool_create_info, VKE_NULLPTR, &_command_pool));
}

void vke::priv::VulkanEngineBase::_create_command_buffer()
{
    const u32 image_count = _swapchain.getImageCount();

    _command_buffer.resize(image_count);

    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};

    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = _command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = image_count;

    VKE_ASSERT(vkAllocateCommandBuffers(_device, &command_buffer_allocate_info, _command_buffer.data()));
}

void vke::priv::VulkanEngineBase::_create_synchronization_objects()
{
    VkFenceCreateInfo fence_create_info = {};

    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    _wait_fences.resize(_swapchain.getImageCount());

    for (auto &fence : _wait_fences) {
        VKE_ASSERT(vkCreateFence(_device, &fence_create_info, VKE_NULLPTR, &fence));
    }
}

void vke::priv::VulkanEngineBase::_create_depth_stencil()
{
    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = _depth_format;
    image_create_info.extent = {_size.width, _size.height, 1};
    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VKE_ASSERT(vkCreateImage(_device, &image_create_info, VKE_NULLPTR, &_depth_stencil._image));

    VkMemoryRequirements memory_requirements = {};
    vkGetImageMemoryRequirements(_device, _depth_stencil._image, &memory_requirements);

    VkMemoryAllocateInfo memory_allocate_info = {};
    memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memory_allocate_info.allocationSize = memory_requirements.size;
    memory_allocate_info.memoryTypeIndex = _vulkan_device->getMemoryType(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VKE_ASSERT(vkAllocateMemory(_device, &memory_allocate_info, VKE_NULLPTR, &_depth_stencil._memory));
    VKE_ASSERT(vkBindImageMemory(_device, _depth_stencil._image, _depth_stencil._memory, 0));

    VkImageViewCreateInfo image_view_create_info = {};
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.image = _depth_stencil._image;
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.format = _depth_format;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = 1;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (_depth_format >= VK_FORMAT_D16_UNORM_S8_UINT) {
        image_view_create_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    VKE_ASSERT(vkCreateImageView(_device, &image_view_create_info, VKE_NULLPTR, &_depth_stencil._image_view));
}

void vke::priv::VulkanEngineBase::_create_pipeline_cache()
{
    VkPipelineCacheCreateInfo pipeline_cache_create_info = {};

    pipeline_cache_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VKE_ASSERT(vkCreatePipelineCache(_device, &pipeline_cache_create_info, VKE_NULLPTR, &_pipeline_cache));
}

void vke::priv::VulkanEngineBase::_create_render_pass()
{
    // clang-format off
    const VkAttachmentDescription attachments[2] = {
        {
            .flags = {},
            .format = _swapchain.getColor()._format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        },
        {
            .flags = {},
            .format = _depth_format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        },
    };
    const VkAttachmentReference color_refs = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    const VkAttachmentReference depth_ref = {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    const VkSubpassDescription subpass_desc = {
        .flags = {},
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = VKE_NULLPTR,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_refs,
        .pResolveAttachments = VKE_NULLPTR,
        .pDepthStencilAttachment = &depth_ref,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = VKE_NULLPTR
    };
    const VkSubpassDependency subpass_dependency[2] = {
        {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
            .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
        },
        {
            .srcSubpass = 0,
            .dstSubpass = VK_SUBPASS_EXTERNAL,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
            .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
        }
    };
    // clang-format on

    VkRenderPassCreateInfo render_pass_create_info = {};
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount = 2;
    render_pass_create_info.pAttachments = attachments;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass_desc;
    render_pass_create_info.dependencyCount = 2;
    render_pass_create_info.pDependencies = subpass_dependency;

    VKE_ASSERT(vkCreateRenderPass(_device, &render_pass_create_info, VKE_NULLPTR, &_render_pass));
}

void vke::priv::VulkanEngineBase::_create_framebuffer()
{
    VkImageView attachments[2];
    attachments[1] = _depth_stencil._image_view;

    VkFramebufferCreateInfo framebuffer_create_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = VKE_NULLPTR,
        .flags = {},
        .renderPass = _render_pass,
        .attachmentCount = 2,
        .pAttachments = attachments,
        .width = _size.width,
        .height = _size.height,
        .layers = 1,
    };

    _framebuffers.resize(_swapchain.getImageCount());
    for (u32 i = 0; i < _framebuffers.size(); ++i) {
        attachments[0] = _swapchain.getBuffers()[i]._view;
        VKE_ASSERT(vkCreateFramebuffer(_device, &framebuffer_create_info, VKE_NULLPTR, &_framebuffers[i]));
    }
}

void vke::priv::VulkanEngineBase::_set_viewports(VkCommandBuffer &cmd_buffer)
{
    VkViewport viewports[1];
    VkRect2D scissor_rects[1];

    viewports[0] = {0, 0, static_cast<f32>(_size.width), static_cast<f32>(_size.height), .0f, 1.0f};
    scissor_rects[0] = {.offset = {0, 0}, .extent = {_size.width, _size.height}};
    vkCmdSetViewport(cmd_buffer, 0, 1, viewports);
    vkCmdSetScissor(cmd_buffer, 0, 1, scissor_rects);
}

/**
* destroy
*/

void vke::priv::VulkanEngineBase::_destroy()
{
    VKE_ASSERT(vkQueueWaitIdle(_queue));
    vkDeviceWaitIdle(_device);

    // Destroy resources that depend on the device
    _context.reset();
    _pipelines.reset();
    _vertex_descriptor.reset();
    _descriptor_set.reset();

    // Clean up other Vulkan resources
    VKE_SAFE_CLEAN(_render_pass, vkDestroyRenderPass(_device, _render_pass, VKE_NULLPTR));

    for (auto &shader : _shader_modules) {
        VKE_SAFE_CLEAN(shader, vkDestroyShaderModule(_device, shader, VKE_NULLPTR));
    }

    VKE_SAFE_CLEAN(_pipeline_cache, vkDestroyPipelineCache(_device, _pipeline_cache, VKE_NULLPTR));
    VKE_SAFE_CLEAN(_semaphores._presentation, vkDestroySemaphore(_device, _semaphores._presentation, VKE_NULLPTR));
    VKE_SAFE_CLEAN(_semaphores._rendering, vkDestroySemaphore(_device, _semaphores._rendering, VKE_NULLPTR));

    _swapchain.destroy();
    _destroy_command_buffer();
    _destroy_framebuffer();
    _destroy_depth_stencil();
    _destroy_fences();

    // Destroy the device
    _vulkan_device.reset();

    // Destroy the instance
    VKE_SAFE_CLEAN(_instance, vkDestroyInstance(_instance, VKE_NULLPTR));
    _prepared = false;
}

void vke::priv::VulkanEngineBase::_destroy_fences()
{
    if (_wait_fences.empty()) {
        return;
    }

    for (auto &fence : _wait_fences) {
        vkDestroyFence(_device, fence, VKE_NULLPTR);
    }
}

void vke::priv::VulkanEngineBase::_destroy_command_buffer()
{
    if (_command_buffer.empty()) {
        return;
    }

    vkFreeCommandBuffers(_device, _command_pool, static_cast<u32>(_command_buffer.size()), _command_buffer.data());
}

void vke::priv::VulkanEngineBase::_destroy_framebuffer()
{
    if (_framebuffers.empty()) {
        return;
    }

    for (auto &framebuffer : _framebuffers) {
        vkDestroyFramebuffer(_device, framebuffer, VKE_NULLPTR);
    }
}

void vke::priv::VulkanEngineBase::_destroy_depth_stencil()
{
    if (_depth_stencil._image_view == VKE_NULLPTR) {
        return;
    }

    vkDestroyImageView(_device, _depth_stencil._image_view, VKE_NULLPTR);
    vkDestroyImage(_device, _depth_stencil._image, VKE_NULLPTR);
    vkFreeMemory(_device, _depth_stencil._memory, VKE_NULLPTR);
    _depth_stencil = {};
}

/**
* draw
*/

void vke::priv::VulkanEngineBase::_resize_window()
{
    if (!_prepared) {
        return;
    }

    _prepared = false;
    vkDeviceWaitIdle(_device);

    _swapchain.create(_size, false);

    _destroy_depth_stencil();
    _create_depth_stencil();

    _destroy_framebuffer();
    _create_framebuffer();

    _destroy_command_buffer();
    _create_command_buffer();
    build_command_buffer();

    vkDeviceWaitIdle(_device);
    _prepared = true;
}

void vke::priv::VulkanEngineBase::_acquire_frame()
{
    if (_paused || !_prepared) {
        return;
    }

    const auto error = _swapchain.next(_semaphores._presentation, &_current_buffer);

    if (error == VK_SUBOPTIMAL_KHR || error == VK_ERROR_OUT_OF_DATE_KHR) {
        std::cout << "VulkanEngineBase::_acquire_frame: Swapchain out of date or suboptimal, resizing window..." << std::endl;
        _resize_window();
        return _acquire_frame();
    } else if (error != VK_SUCCESS) {
        throw vke::exception::RuntimeError("Vulkan fatal error", VKE_GET_ERROR_STRING(error), " file: ", __FILE__, ", line: ", __LINE__);
    }
    VKE_ASSERT(vkQueueWaitIdle(_queue));
}

void vke::priv::VulkanEngineBase::_submit_frame()
{
    if (_paused) {
        return;
    }

    const auto error = _swapchain.queue(_queue, _current_buffer, _semaphores._rendering);

    if (error == VK_SUBOPTIMAL_KHR || error == VK_ERROR_OUT_OF_DATE_KHR) {
        std::cout << "VulkanEngineBase::_submit_frame: Swapchain out of date or suboptimal, resizing window..." << std::endl;
        _resize_window();
        return _submit_frame();
    } else if (error != VK_SUCCESS) {
        throw vke::exception::RuntimeError("Vulkan fatal error", VKE_GET_ERROR_STRING(error), " file: ", __FILE__, ", line: ", __LINE__);
    }
    VKE_ASSERT(vkQueueWaitIdle(_queue));
}
