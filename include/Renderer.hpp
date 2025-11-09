#pragma once

#include "vulkan_backend/commands/CommandBuffer.hpp"
#include "vulkan_backend/commands/CommandPool.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/core/Instance.hpp"
#include "vulkan_backend/pipeline/Pipeline.hpp"
#include "vulkan_backend/pipeline/RenderPass.hpp"
#include "vulkan_backend/resources/Framebuffer.hpp"
#include "vulkan_backend/surface/Surface.hpp"
#include "vulkan_backend/surface/SwapChain.hpp"
#include "vulkan_backend/sync/SyncManager.hpp"

#include <memory>
#include <vector>

struct GLFWwindow;

namespace lumen {

class Renderer
{
    public:
        explicit Renderer(GLFWwindow *window);
        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        void draw_frame();
        void notify_framebuffer_resized();

    private:
        void _init_vulkan();
        void _create_framebuffers();
        void _cleanup_swap_chain();
        void _recreate_swap_chain();

        GLFWwindow *_window;

        std::unique_ptr<Instance> _instance;
        std::unique_ptr<Surface> _surface;
        std::unique_ptr<Device> _device;
        std::unique_ptr<SwapChain> _swapChain;
        std::unique_ptr<RenderPass> _renderPass;
        std::unique_ptr<Pipeline> _pipeline;
        std::unique_ptr<CommandPool> _commandPool;
        std::vector<std::unique_ptr<CommandBuffer>> _commandBuffers;
        std::vector<std::unique_ptr<Framebuffer>> _framebuffers;
        std::unique_ptr<SyncManager> _syncManager;

        uint32_t _currentFrame = 0;
        bool _framebufferResized = false;
};

}// namespace lumen
