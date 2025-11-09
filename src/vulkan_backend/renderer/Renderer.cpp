#include <vulkan_backend/commands/CommandQueue.hpp>
#include <vulkan_backend/renderer/RenderWindow.hpp>
#include <vulkan_backend/renderer/Renderer.hpp>
#include <vulkan_backend/utils/Config.hpp>
#include <vulkan_backend/utils/Result.hpp>

#include <stdexcept>

/**
 * public
 */
// clang-format off

lumen::Renderer::Renderer(RenderWindow &window) : _window(window)
{
    _init_vulkan();
    _window.set_framebuffer_resize_callback(
    [this](int __attribute__((unused)) _w, int __attribute__((unused)) _h)
    {
        this->_framebufferResized = true;
    });
}

lumen::Renderer::~Renderer()
{
    vkDeviceWaitIdle(_device->logicalDevice());
    _cleanup_swap_chain();
}

void lumen::Renderer::draw_frame()
{
    const std::optional<FrameData> frame_data = _begin_frame();

    if (!frame_data.has_value()) {
        return;
    }

    _record_draw_commands(frame_data.value());
    _end_frame(frame_data.value());
}

/**
 * private
 */

std::optional<lumen::Renderer::FrameData> lumen::Renderer::_begin_frame()
{
    auto &inFlightFence = _syncManager->getInFlightFence(_currentFrame);
    inFlightFence.wait();

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->logicalDevice(), _swapChain->handle(), UINT64_MAX,
        _syncManager->getImageAvailableSemaphore(_currentFrame).handle(), VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        _recreate_swap_chain();
        return std::nullopt;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    inFlightFence.reset();

    auto &cmdBuffer = _commandBuffers[_currentFrame];
    cmdBuffer->reset();
    cmdBuffer->begin();

    return FrameData{cmdBuffer->handle(), imageIndex};
}

void lumen::Renderer::_record_draw_commands(const FrameData &frame)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _renderPass->handle();
    renderPassInfo.framebuffer = _framebuffers[frame.image_index]->handle();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChain->getExtent();
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(frame.command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(frame.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->handle());

    VkViewport viewport{};
    viewport.width = static_cast<float>(_swapChain->getExtent().width);
    viewport.height = static_cast<float>(_swapChain->getExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(frame.command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.extent = _swapChain->getExtent();
    vkCmdSetScissor(frame.command_buffer, 0, 1, &scissor);

    vkCmdDraw(frame.command_buffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(frame.command_buffer);

    vk_check(vkEndCommandBuffer(frame.command_buffer), "failed to record command buffer!");
}

void lumen::Renderer::_end_frame(const FrameData &frame)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {_syncManager->getImageAvailableSemaphore(_currentFrame).handle()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &frame.command_buffer;

    VkSemaphore signalSemaphores[] = {_syncManager->getRenderFinishedSemaphore(frame.image_index).handle()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    auto &inFlightFence = _syncManager->getInFlightFence(_currentFrame);
    vk_check(vkQueueSubmit(_device->getGraphicsQueue().handle(), 1, &submitInfo, inFlightFence.handle()),
        "failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {_swapChain->handle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &frame.image_index;

    VkResult result = vkQueuePresentKHR(_device->getPresentQueue().handle(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
        _framebufferResized = false;
        _recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    _currentFrame = (_currentFrame + 1) % config::MAX_FRAMES_IN_FLIGHT;
}

void lumen::Renderer::_init_vulkan()
{
    _instance = std::make_unique<Instance>("Lumen App");
    _surface = std::make_unique<Surface>(*_instance, _window.get_native_handle());
    _device = std::make_unique<Device>(*_instance, *_surface);
    _swapChain = std::make_unique<SwapChain>(*_instance, *_device, *_surface, _window.get_native_handle());
    _renderPass = std::make_unique<RenderPass>(*_device, *_swapChain);
    _pipeline = std::make_unique<Pipeline>(*_device, *_renderPass, "assets/shaders/triangle.vert.spv", "assets/shaders/triangle.frag.spv");
    _commandPool = std::make_unique<CommandPool>(*_device, _device->getQueueFamilyIndices().graphicsFamily.value());

    _create_framebuffers();

    _commandBuffers.resize(config::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < config::MAX_FRAMES_IN_FLIGHT; ++i) {
        _commandBuffers[i] = std::make_unique<CommandBuffer>(*_device, *_commandPool);
    }

    _syncManager = std::make_unique<SyncManager>(*_device, config::MAX_FRAMES_IN_FLIGHT, _swapChain->getImageCount());
}

void lumen::Renderer::_create_framebuffers()
{
    _framebuffers.resize(_swapChain->getImageCount());
    for (size_t i = 0; i < _swapChain->getImageCount(); ++i) {
        _framebuffers[i] = std::make_unique<Framebuffer>(*_device, *_renderPass, *_swapChain->getImageViews()[i], _swapChain->getExtent());
    }
}

void lumen::Renderer::_cleanup_swap_chain()
{
    _framebuffers.clear();
    _swapChain.reset();
}

void lumen::Renderer::_recreate_swap_chain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(_window.get_native_handle(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_window.get_native_handle(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(_device->logicalDevice());
    _cleanup_swap_chain();

    _swapChain = std::make_unique<SwapChain>(*_instance, *_device, *_surface, _window.get_native_handle());
    _renderPass = std::make_unique<RenderPass>(*_device, *_swapChain);
    _pipeline = std::make_unique<Pipeline>(*_device, *_renderPass, "assets/shaders/triangle.vert.spv", "assets/shaders/triangle.frag.spv");
    _create_framebuffers();
}
