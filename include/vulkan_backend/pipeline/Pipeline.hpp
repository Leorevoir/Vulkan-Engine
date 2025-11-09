#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace lumen {

class Device;
class RenderPass;

class Pipeline
{
    public:
        Pipeline(Device &device, const RenderPass &renderPass, const std::string &vertShaderPath, const std::string &fragShaderPath);
        ~Pipeline() noexcept;

        VkPipeline handle() const noexcept;
        VkPipelineLayout getLayout() const noexcept;

    private:
        VkPipeline _graphicsPipeline;
        VkPipelineLayout _pipelineLayout;
        Device &_device;
};

}// namespace lumen
