#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

#include <VKE/Graphics/Shader/VulkanShader.hpp>

#include <memory>

namespace vke {

namespace priv {

class VKE_API VulkanPipelines
{
    public:
        explicit VulkanPipelines(VkDevice &device, VkPipelineVertexInputStateCreateInfo &state, VkPipelineCache cache);
        ~VulkanPipelines() = default;

        void createBase(const VkPipelineLayout &pipeline_layout, const VkRenderPass &render_pass);

        void create(VulkanShader *shader, VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL);
        void create(VulkanShader *shader, VkRenderPass render_pass, VkPolygonMode mode = VK_POLYGON_MODE_FILL);
        void create(std::shared_ptr<VulkanShader> shader, VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL);
        void create(std::shared_ptr<VulkanShader> shader, VkRenderPass render_pass, VkPolygonMode polygon_mode);

    private:
        VkDevice _device;

        VkGraphicsPipelineCreateInfo _pipeline_info = {};
        VkPipelineVertexInputStateCreateInfo _input_state = {};
        VkPipelineCache _pipeline_cache = {};

        VkPipelineInputAssemblyStateCreateInfo _input_assembly_state = {};
        VkPipelineRasterizationStateCreateInfo _rasterization_state = {};
        VkPipelineColorBlendAttachmentState _blend_attachment_state = {};
        VkPipelineColorBlendStateCreateInfo _color_blend_state = {};
        VkPipelineDepthStencilStateCreateInfo _depth_stencil_state = {};
        VkPipelineViewportStateCreateInfo _viewport_state = {};
        VkPipelineMultisampleStateCreateInfo _multisample_state = {};
        std::vector<VkDynamicState> _dynamic_state_enables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH};
        VkPipelineDynamicStateCreateInfo _dynamic_state = {};
        std::array<VkPipelineShaderStageCreateInfo, 2> _shader_stages;
};

}// namespace priv

}// namespace vke
