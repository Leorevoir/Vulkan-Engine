#include <VKE/Error.hpp>
#include <VKE/Graphics/Engine/Base/VulkanEngineBase.hpp>

/**
* destroy
*/

void vke::priv::VulkanEngineBase::_destroy()
{
    VKE_ASSERT(vkQueueWaitIdle(_queue));
    vkDeviceWaitIdle(_device);

    /** @brief destroy resources that depend on the device */
    _context.reset();
    _pipelines.reset();
    _vertex_descriptor.reset();
    _descriptor_set.reset();

    /** @brief clean up other Vulkan resources */
    VKE_SAFE_CLEAN(_render_pass, vkDestroyRenderPass(_device, _render_pass, VKE_NULLPTR));
    VKE_SAFE_CLEAN(_pipeline_layout, vkDestroyPipelineLayout(_device, _pipeline_layout, VKE_NULLPTR));

    for (auto &shader : _shader_modules) {
        VKE_SAFE_CLEAN(shader, vkDestroyShaderModule(_device, shader, VKE_NULLPTR));
    }

    VKE_SAFE_CLEAN(_pipeline_cache, vkDestroyPipelineCache(_device, _pipeline_cache, VKE_NULLPTR));

    _destroy_semaphores();

    _swapchain.destroy();
    _destroy_command_buffer();
    _destroy_framebuffer();
    _destroy_depth_stencil();
    _destroy_fences();

    /** @brief destroy the command pool */
    VKE_SAFE_CLEAN(_command_pool, vkDestroyCommandPool(_device, _command_pool, VKE_NULLPTR));

    /** @brief destroy the device */
    _vulkan_device.reset();

    /** @brief Destroy the instance */
    VKE_SAFE_CLEAN(_instance, vkDestroyInstance(_instance, VKE_NULLPTR));
    _prepared = false;
}

void vke::priv::VulkanEngineBase::_destroy_semaphores()
{
    VKE_SAFE_CLEAN(_semaphores._imageAvailableSemaphore, vkDestroySemaphore(_device, _semaphores._imageAvailableSemaphore, VKE_NULLPTR));
    VKE_SAFE_CLEAN(_semaphores._renderFinishedSemaphore, vkDestroySemaphore(_device, _semaphores._renderFinishedSemaphore, VKE_NULLPTR));
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
