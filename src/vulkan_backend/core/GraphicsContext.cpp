#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/renderer/RenderWindow.hpp>

/**
 * public
 */

lumen::GraphicsContext::GraphicsContext(RenderWindow &window)
{
    _init_vulkan(window);
}

lumen::GraphicsContext::~GraphicsContext()
{
    /* __dtor__ */
}

/**
 * getters
 */

lumen::Instance &lumen::GraphicsContext::get_instance() const
{
    return *_instance;
}

lumen::Device &lumen::GraphicsContext::get_device() const
{
    return *_device;
}

lumen::Surface &lumen::GraphicsContext::get_surface() const
{
    return *_surface;
}

lumen::CommandPool &lumen::GraphicsContext::get_command_pool() const
{
    return *_commandPool;
}

/**
 * private
 */

void lumen::GraphicsContext::_init_vulkan(RenderWindow &window)
{
    _instance = std::make_unique<Instance>("Lumen App");
    _surface = std::make_unique<Surface>(*_instance, window.get_native_handle());
    _device = std::make_unique<Device>(*_instance, *_surface);
    _commandPool = std::make_unique<CommandPool>(*_device, _device->getQueueFamilyIndices().graphicsFamily.value());
}
