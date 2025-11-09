#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/commands/CommandBuffer.hpp>
#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/pipeline/Pipeline.hpp>
#include <vulkan_backend/pipeline/RenderPass.hpp>
#include <vulkan_backend/resources/Framebuffer.hpp>
#include <vulkan_backend/surface/SwapChain.hpp>
#include <vulkan_backend/sync/SyncManager.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace lumen {

class RenderWindow;

class Renderer
{
    public:
        explicit Renderer(GraphicsContext &context, RenderWindow &window);
        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;
        Renderer(Renderer &&) = delete;
        Renderer &operator=(Renderer &&) = delete;

        void draw_frame(const Pipeline &pipeline);
        const RenderPass &get_render_pass() const;

    private:
        struct FrameData {
                VkCommandBuffer command_buffer;
                uint32_t image_index;
        };

        void _init_renderer();
        void _create_framebuffers();
        void _cleanup_swap_chain();
        void _recreate_swap_chain();

        std::optional<FrameData> _begin_frame();
        void _record_draw_commands(const FrameData &frame, const Pipeline &pipeline);
        void _end_frame(const FrameData &frame);

        GraphicsContext &_context;
        RenderWindow &_window;

        std::unique_ptr<SwapChain> _swapChain;
        std::unique_ptr<RenderPass> _renderPass;
        std::vector<std::unique_ptr<CommandBuffer>> _commandBuffers;
        std::vector<std::unique_ptr<Framebuffer>> _framebuffers;
        std::unique_ptr<SyncManager> _syncManager;

        uint32_t _currentFrame = 0;
        bool _framebufferResized = false;
};

}// namespace lumen
