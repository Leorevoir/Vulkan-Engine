#pragma once

#include <memory>

#include <vulkan_backend/commands/CommandPool.hpp>
#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/core/Instance.hpp>
#include <vulkan_backend/memory/Allocator.hpp>
#include <vulkan_backend/surface/Surface.hpp>

namespace lumen {

class RenderWindow;

class GraphicsContext
{
    public:
        explicit GraphicsContext(RenderWindow &window);
        ~GraphicsContext() noexcept;

        GraphicsContext(const GraphicsContext &) = delete;
        GraphicsContext &operator=(const GraphicsContext &) = delete;

        Instance &get_instance() const noexcept;
        Device &get_device() const noexcept;
        Surface &get_surface() const noexcept;
        CommandPool &get_command_pool() const noexcept;
        Allocator &get_allocator() const noexcept;

    private:
        void _init_vulkan(RenderWindow &window);

        std::unique_ptr<Instance> _instance;
        std::unique_ptr<Surface> _surface;
        std::unique_ptr<Device> _device;
        std::unique_ptr<CommandPool> _commandPool;
        std::unique_ptr<Allocator> _allocator;
};

}// namespace lumen
