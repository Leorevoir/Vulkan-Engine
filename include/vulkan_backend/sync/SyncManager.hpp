#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/sync/Fence.hpp>
#include <vulkan_backend/sync/Semaphore.hpp>

#include <memory>
#include <vector>

namespace lumen {

class Device;

class SyncManager
{
    public:
        SyncManager(Device &device, size_t maxFramesInFlight, uint32_t imageCount);
        ~SyncManager() = default;

        const Fence &getInFlightFence(size_t frameIndex) const noexcept;
        const Semaphore &getImageAvailableSemaphore(size_t frameIndex) const noexcept;
        const Semaphore &getRenderFinishedSemaphore(size_t imageIndex) const noexcept;

    private:
        std::vector<std::unique_ptr<Semaphore>> _imageAvailableSemaphores;
        std::vector<std::unique_ptr<Semaphore>> _renderFinishedSemaphores;
        std::vector<std::unique_ptr<Fence>> _inFlightFences;
};

}// namespace lumen
