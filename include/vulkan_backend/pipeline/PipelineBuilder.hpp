#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class PipelineBuilder
{
    public:
        PipelineBuilder(Device &device);
        ~PipelineBuilder() = default;

        // TODO: Add methods to configure vertex input, shaders, rasterizer, etc.

        VkPipeline build(VkRenderPass renderPass, VkPipelineLayout layout);

    private:
        Device &_device;
};

}// namespace lumen
