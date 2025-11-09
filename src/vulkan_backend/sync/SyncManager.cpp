#include "vulkan_backend/sync/SyncManager.hpp"
#include "vulkan_backend/core/Device.hpp"

/**
* public
*/

lumen::SyncManager::SyncManager(Device &device, size_t maxFramesInFlight)
{
    _imageAvailableSemaphores.resize(maxFramesInFlight);
    _renderFinishedSemaphores.resize(maxFramesInFlight);
    _inFlightFences.resize(maxFramesInFlight);

    for (size_t i = 0; i < maxFramesInFlight; i++) {
        _imageAvailableSemaphores[i] = std::make_unique<Semaphore>(device);
        _renderFinishedSemaphores[i] = std::make_unique<Semaphore>(device);
        _inFlightFences[i] = std::make_unique<Fence>(device, true);
    }
}

/**
* getter
*/

const lumen::Semaphore &lumen::SyncManager::getImageAvailableSemaphore(size_t frameIndex) const noexcept
{
    return *_imageAvailableSemaphores[frameIndex];
}

const lumen::Semaphore &lumen::SyncManager::getRenderFinishedSemaphore(size_t frameIndex) const noexcept
{
    return *_renderFinishedSemaphores[frameIndex];
}

const lumen::Fence &lumen::SyncManager::getInFlightFence(size_t frameIndex) const noexcept
{
    return *_inFlightFences[frameIndex];
}
