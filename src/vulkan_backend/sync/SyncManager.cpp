#include "vulkan_backend/sync/SyncManager.hpp"
#include "vulkan_backend/core/Device.hpp"

/**
* public
*/

lumen::SyncManager::SyncManager(Device &device, size_t maxFramesInFlight, uint32_t imageCount)
{
    _imageAvailableSemaphores.resize(maxFramesInFlight);
    _inFlightFences.resize(maxFramesInFlight);
    _renderFinishedSemaphores.resize(imageCount);

    for (size_t i = 0; i < maxFramesInFlight; ++i) {
        _imageAvailableSemaphores[i] = std::make_unique<Semaphore>(device);
        _inFlightFences[i] = std::make_unique<Fence>(device, true);
    }

    for (size_t i = 0; i < imageCount; ++i) {
        _renderFinishedSemaphores[i] = std::make_unique<Semaphore>(device);
    }
}

/**
* getter
*/

const lumen::Semaphore &lumen::SyncManager::getImageAvailableSemaphore(size_t frameIndex) const noexcept
{
    return *_imageAvailableSemaphores[frameIndex];
}

const lumen::Semaphore &lumen::SyncManager::getRenderFinishedSemaphore(size_t imageIndex) const noexcept
{
    return *_renderFinishedSemaphores[imageIndex];
}

const lumen::Fence &lumen::SyncManager::getInFlightFence(size_t frameIndex) const noexcept
{
    return *_inFlightFences[frameIndex];
}
