#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class Semaphore
{
    public:
        Semaphore(Device &device);
        ~Semaphore() noexcept;

        VkSemaphore handle() const noexcept;

    private:
        VkSemaphore _semaphore;
        Device &_device;
};

}// namespace lumen
