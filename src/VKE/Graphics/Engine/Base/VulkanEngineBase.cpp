#include <VKE/Error.hpp>
#include <VKE/Graphics/Engine/Base/VulkanEngineBase.hpp>

#include <chrono>

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
    build_command_buffer();

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

void vke::priv::VulkanEngineBase::build_command_buffer()
{
    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    for (size_t i = 0; i < _command_buffer.size(); ++i) {

        VKE_ASSERT(vkBeginCommandBuffer(_command_buffer[i], &cmdBufInfo));

        buildCommandBufferBeforeRenderPass(_command_buffer[i]);

        {
            VkClearValue clear_values[2];
            clear_values[0].color = {{0.1f, 0.2f, 0.3f, 1.0f}};
            clear_values[1].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo render_pass_begin_info = {};
            render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            render_pass_begin_info.renderPass = _render_pass;
            render_pass_begin_info.renderArea.offset.x = 0;
            render_pass_begin_info.renderArea.offset.y = 0;
            render_pass_begin_info.renderArea.extent.width = _size.width;
            render_pass_begin_info.renderArea.extent.height = _size.height;
            render_pass_begin_info.clearValueCount = 2;
            render_pass_begin_info.pClearValues = clear_values;
            render_pass_begin_info.framebuffer = _framebuffers[i];

            vkCmdBeginRenderPass(_command_buffer[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
            VkDeviceSize offsets[1] = {0};
            vkCmdBindDescriptorSets(_command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_layout, 0, 1, &(_descriptor_set->get(0)), 0, NULL);

            _set_viewports(_command_buffer[i]);
            drawObjects(_command_buffer[i]);

            vkCmdEndRenderPass(_command_buffer[i]);
        }

        buildCommandBufferAfterRenderPass(_command_buffer[i]);

        VKE_ASSERT(vkEndCommandBuffer(_command_buffer[i]));
    }
    vkQueueWaitIdle(_queue);
}

/**
* private
*/

/**
* draw
*/

void vke::priv::VulkanEngineBase::_set_viewports(VkCommandBuffer &cmd_buffer)
{
    VkViewport viewports[1];
    VkRect2D scissor_rects[1];

    viewports[0] = {0, 0, static_cast<f32>(_size.width), static_cast<f32>(_size.height), .0f, 1.0f};
    scissor_rects[0] = {.offset = {0, 0}, .extent = {_size.width, _size.height}};

    vkCmdSetViewport(cmd_buffer, 0, 1, viewports);
    vkCmdSetScissor(cmd_buffer, 0, 1, scissor_rects);
}

void vke::priv::VulkanEngineBase::_resize_window()
{
    if (!_prepared) {
        return;
    }

    _prepared = false;
    vkDeviceWaitIdle(_device);

    /** INFO: destroy the semaphores & recreate it to avoid already-signaled */
    _destroy_semaphores();

    _swapchain.create(_size, false);

    _destroy_depth_stencil();
    _create_depth_stencil();

    _destroy_framebuffer();
    _create_framebuffer();

    _destroy_command_buffer();
    _create_command_buffer();
    build_command_buffer();

    _create_semaphores();

    vkDeviceWaitIdle(_device);
    _prepared = true;
}

void vke::priv::VulkanEngineBase::_acquire_frame()
{
    if (_paused || !_prepared) {
        return;
    }

    VkResult error = _swapchain.next(_semaphores._imageAvailableSemaphore, &_current_buffer);

    if (error == VK_SUBOPTIMAL_KHR || error == VK_ERROR_OUT_OF_DATE_KHR) {
        _resize_window();
        error = _swapchain.next(_semaphores._imageAvailableSemaphore, &_current_buffer);
    }

    VKE_ASSERT(error);
    VKE_ASSERT(vkQueueWaitIdle(_queue));
}

void vke::priv::VulkanEngineBase::_submit_frame()
{
    if (_paused) {
        return;
    }

    VkResult error = _swapchain.queue(_queue, _current_buffer, _semaphores._renderFinishedSemaphore);

    if (error == VK_SUBOPTIMAL_KHR || error == VK_ERROR_OUT_OF_DATE_KHR) {
        _resize_window();
        return;
    }

    VKE_ASSERT(error);
    VKE_ASSERT(vkQueueWaitIdle(_queue));
}
