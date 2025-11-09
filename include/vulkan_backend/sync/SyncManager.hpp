#pragma once

#include "Fence.hpp"
#include "Semaphore.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class SyncManager
{
    public:
        SyncManager(Device &device, size_t maxFramesInFlight);
        ~SyncManager() = default;

        const Fence &getInFlightFence(size_t frameIndex) const noexcept;
        const Semaphore &getImageAvailableSemaphore(size_t frameIndex) const noexcept;
        const Semaphore &getRenderFinishedSemaphore(size_t frameIndex) const noexcept;

    private:
        std::vector<std::unique_ptr<Semaphore>> _imageAvailableSemaphores;
        std::vector<std::unique_ptr<Semaphore>> _renderFinishedSemaphores;
        std::vector<std::unique_ptr<Fence>> _inFlightFences;
};

}// namespace lumen
