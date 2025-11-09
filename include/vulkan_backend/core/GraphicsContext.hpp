#pragma once

#include <memory>

#include <vulkan_backend/commands/CommandPool.hpp>
#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/core/Instance.hpp>
#include <vulkan_backend/surface/Surface.hpp>

namespace lumen {

class RenderWindow;

class GraphicsContext
{
    public:
        explicit GraphicsContext(RenderWindow &window);
        ~GraphicsContext();

        GraphicsContext(const GraphicsContext &) = delete;
        GraphicsContext &operator=(const GraphicsContext &) = delete;

        Instance &get_instance() const;
        Device &get_device() const;
        Surface &get_surface() const;
        CommandPool &get_command_pool() const;

    private:
        void _init_vulkan(RenderWindow &window);

        std::unique_ptr<Instance> _instance;
        std::unique_ptr<Surface> _surface;
        std::unique_ptr<Device> _device;
        std::unique_ptr<CommandPool> _commandPool;
};

}// namespace lumen
