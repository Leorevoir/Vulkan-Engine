#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class Fence
{
    public:
        Fence(Device &device, bool signaled = false);
        ~Fence();

        void wait() const noexcept;
        void reset() const noexcept;

        VkFence handle() const noexcept;

    private:
        VkFence _fence;
        Device &_device;
};

}// namespace lumen
