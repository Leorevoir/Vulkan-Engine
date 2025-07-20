#include <VKE/Graphics/Swapchain/SwapchainBase.hpp>

/**
* public
*/

u32 vke::priv::SwapchainBase::getQueueIndex() const
{
    return _queue_index;
}

u32 vke::priv::SwapchainBase::getImageCount() const
{
    return _image_count;
}

const vke::priv::SwapChainColor &vke::priv::SwapchainBase::getColor() const
{
    return _color;
}

const std::vector<vke::priv::SwapchainBuffers> &vke::priv::SwapchainBase::getBuffers() const
{
    return _buffers;
}

/**
* protected
*/
