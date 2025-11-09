#pragma once

#include <vulkan_backend/Backend.hpp>

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
