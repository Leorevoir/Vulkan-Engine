#include "Renderer.hpp"
#include "vulkan_backend/commands/CommandQueue.hpp"
#include "vulkan_backend/utils/Config.hpp"
#include "vulkan_backend/utils/Result.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

/**
* public
*/

lumen::Renderer::Renderer(GLFWwindow *window) : _window(window)
{
    _init_vulkan();
}

lumen::Renderer::~Renderer()
{
    vkDeviceWaitIdle(_device->logicalDevice());

    _cleanup_swap_chain();

    _syncManager.reset();
    _commandBuffers.clear();
    _commandPool.reset();
    _pipeline.reset();
    _renderPass.reset();
    _device.reset();
    _surface.reset();
    _instance.reset();
}

void lumen::Renderer::notify_framebuffer_resized()
{
    _framebufferResized = true;
}

void lumen::Renderer::draw_frame()
{
    auto &inFlightFence = _syncManager->getInFlightFence(_currentFrame);
    inFlightFence.wait();

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->logicalDevice(), _swapChain->handle(), UINT64_MAX,
        _syncManager->getImageAvailableSemaphore(_currentFrame).handle(), VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        _recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    inFlightFence.reset();

    auto &cmdBuffer = _commandBuffers[_currentFrame];
    cmdBuffer->reset();
    cmdBuffer->begin();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _renderPass->handle();
    renderPassInfo.framebuffer = _framebuffers[imageIndex]->handle();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChain->getExtent();
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cmdBuffer->handle(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmdBuffer->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->handle());

    VkViewport viewport{};
    viewport.width = static_cast<float>(_swapChain->getExtent().width);
    viewport.height = static_cast<float>(_swapChain->getExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer->handle(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.extent = _swapChain->getExtent();
    vkCmdSetScissor(cmdBuffer->handle(), 0, 1, &scissor);

    vkCmdDraw(cmdBuffer->handle(), 3, 1, 0, 0);
    vkCmdEndRenderPass(cmdBuffer->handle());
    cmdBuffer->end();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {_syncManager->getImageAvailableSemaphore(_currentFrame).handle()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer rawCmdBuffer = cmdBuffer->handle();
    submitInfo.pCommandBuffers = &rawCmdBuffer;

    VkSemaphore signalSemaphores[] = {_syncManager->getRenderFinishedSemaphore(imageIndex).handle()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vk_check(vkQueueSubmit(_device->getGraphicsQueue().handle(), 1, &submitInfo, inFlightFence.handle()),
        "failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {_swapChain->handle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(_device->getPresentQueue().handle(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
        _framebufferResized = false;
        _recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    _currentFrame = (_currentFrame + 1) % config::MAX_FRAMES_IN_FLIGHT;
}

/**
* private
*/

void lumen::Renderer::_init_vulkan()
{
    _instance = std::make_unique<Instance>("Lumen App");
    _surface = std::make_unique<Surface>(*_instance, _window);
    _device = std::make_unique<Device>(*_instance, *_surface);
    _swapChain = std::make_unique<SwapChain>(*_instance, *_device, *_surface, _window);
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

    glfwGetFramebufferSize(_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(_device->logicalDevice());

    _cleanup_swap_chain();

    _swapChain = std::make_unique<SwapChain>(*_instance, *_device, *_surface, _window);
    _create_framebuffers();
}
