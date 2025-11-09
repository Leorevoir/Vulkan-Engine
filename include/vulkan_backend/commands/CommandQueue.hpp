#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class CommandQueue
{
    public:
        CommandQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex) noexcept;
        ~CommandQueue() noexcept = default;

        VkQueue handle() const noexcept;
        uint32_t familyIndex() const noexcept;

    private:
        VkQueue _queue;
        uint32_t _familyIndex;
};

}// namespace lumen
