#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Graphics/Context/VulkanContext.hpp>
#include <VKE/Graphics/VulkanObject.hpp>
#include <VKE/Macros.hpp>

namespace vke {

namespace priv {

class VKE_API VulkanShaderBase : public VulkanObject
{
    public:
        constexpr VulkanShaderBase() = default;
        ~VulkanShaderBase() override;

        void update() override;

        /** getters */
        VkPipeline &getPipeline();
        const std::vector<VkPipelineShaderStageCreateInfo> &getShaderStages() const;
        VkCullModeFlags getCullMode() const;
        VkFrontFace getFrontFace() const;
        VkPipelineVertexInputStateCreateInfo getVertexInputState() const;
        bool isDepthBiasEnabled() const;
        bool isInstanceShader() const;
        bool isOneStage() const;

        /** setters */
        void setCullMode(const VkCullModeFlags &flag);
        void setFrontFace(const VkFrontFace &face);
        void setVertexInputState(const VkPipelineVertexInputStateCreateInfo &vertex_input_state);
        void setDepthBiasEnabled(bool enabled);
        void setOneStage(bool one_stage);

    protected:
        VkPipeline _pipeline = VK_NULL_HANDLE;
        VkCullModeFlags _cull_flags = VK_CULL_MODE_NONE;
        VkFrontFace _front_face = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        VkPipelineVertexInputStateCreateInfo _vertex_input_state = {};

        bool _depth_bias_enabled = false;
        bool _instance_shader = false;
        bool _one_stage = false;

        std::vector<VkShaderModule> _shader_modules;
        std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;

        VkPipelineShaderStageCreateInfo _load(const std::string &filename, const VkShaderStageFlagBits &stage);
};

}// namespace priv

}// namespace vke
