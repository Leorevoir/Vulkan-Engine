#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"
#include <string>

namespace lumen {

class RenderPass;

class Pipeline : public VulkanObject<VkPipeline>
{
    public:
        Pipeline(Device &device, const RenderPass &renderPass, const std::string &vertShaderPath, const std::string &fragShaderPath);
        ~Pipeline() noexcept override;

        VkPipelineLayout getLayout() const noexcept;

    private:
        VkPipelineLayout _pipelineLayout;
};

}// namespace lumen
