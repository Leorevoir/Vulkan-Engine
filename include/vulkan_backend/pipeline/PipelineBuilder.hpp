#pragma once

#include <vulkan_backend/Backend.hpp>

#include <memory>
#include <string>
#include <vector>

namespace lumen {

class Device;
class Pipeline;
class Shader;

class PipelineBuilder
{
    public:
        explicit PipelineBuilder(Device &device) noexcept;
        ~PipelineBuilder() noexcept;

        PipelineBuilder &set_shader_stages(const std::string &vert_path, const std::string &frag_path, bool from_source = false);
        PipelineBuilder &set_vertex_input_state() noexcept;
        PipelineBuilder &set_input_assembly_state() noexcept;
        PipelineBuilder &set_viewport_state() noexcept;
        PipelineBuilder &set_rasterization_state() noexcept;
        PipelineBuilder &set_multisample_state() noexcept;
        PipelineBuilder &set_color_blend_state() noexcept;
        PipelineBuilder &set_dynamic_state(const std::vector<VkDynamicState> &states) noexcept;

        VkPipeline build(VkRenderPass render_pass, VkPipelineLayout *out_pipeline_layout);

    private:
        Device &_device;

        VkPipelineVertexInputStateCreateInfo _vertexInputInfo{};
        VkPipelineInputAssemblyStateCreateInfo _inputAssemblyInfo{};
        VkPipelineViewportStateCreateInfo _viewportInfo{};
        VkPipelineRasterizationStateCreateInfo _rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo _multisampleInfo{};
        VkPipelineColorBlendAttachmentState _colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo _colorBlendInfo{};
        VkPipelineDynamicStateCreateInfo _dynamicStateInfo{};

        std::vector<VkDynamicState> _dynamicStateEnables;
        std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
        std::vector<std::unique_ptr<Shader>> _shaderModules;
        std::vector<VkVertexInputBindingDescription> _bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> _attributeDescriptions;
};

}// namespace lumen
