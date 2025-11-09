#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

namespace lumen {

//TODO: modularize mais bon on fait avec ce qu'on a pour l'instant mddrrrrr (c'est pas propre du tout)

class Application
{
    public:
        Application();
        virtual ~Application();

        void run();

    private:
        void _init_window();
        void _init_vulkan();
        void _main_loop();
        void _draw_frame();
        void _cleanup();
        void _create_framebuffers();
        void _cleanup_swap_chain();
        void _recreate_swap_chain();

        static void _framebuffer_resize_callback(GLFWwindow *window, int width, int height);

        GLFWwindow *_window = nullptr;

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
