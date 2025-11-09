#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;
class CommandPool;

class CommandBuffer
{
    public:
        CommandBuffer(Device &device, const CommandPool &commandPool);
        ~CommandBuffer() = default;

        void begin();
        void end();
        void reset();

        VkCommandBuffer handle() const;

    private:
        VkCommandBuffer _commandBuffer;
        Device &_device;
};

}// namespace lumen
