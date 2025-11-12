#include <utils/maths/Vertex.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/pipeline/PipelineBuilder.hpp>
#include <vulkan_backend/pipeline/Shader.hpp>
#include <vulkan_backend/utils/Result.hpp>

/**
* public
*/
// clang-format off

lumen::PipelineBuilder::PipelineBuilder(Device &device) noexcept : _device(device)
{
    /* __ctor__ */
}

lumen::PipelineBuilder::~PipelineBuilder() noexcept
{
    /* __dtor__ */
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_shader_stages(const std::string &vert_path, const std::string &frag_path)
{
    _shaderModules.clear();
    _shaderStages.clear();

    _shaderModules.push_back(std::make_unique<Shader>(_device, vert_path));
    _shaderModules.push_back(std::make_unique<Shader>(_device, frag_path));

    _shaderStages.push_back({
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = _shaderModules[0]->handle(),
        .pName = "main",
        .pSpecializationInfo = nullptr,
    });
    _shaderStages.push_back({
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = _shaderModules[1]->handle(),
        .pName = "main",
        .pSpecializationInfo = nullptr,
    });

    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_vertex_input_state() noexcept
{
    _bindingDescriptions.resize(1);
    _attributeDescriptions.resize(2);

    _bindingDescriptions[0].binding = 0;
    _bindingDescriptions[0].stride = sizeof(Vertex);
    _bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    _attributeDescriptions[0].binding = 0;
    _attributeDescriptions[0].location = 0; ///<< position
    _attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    _attributeDescriptions[0].offset = offsetof(Vertex, position);

    _attributeDescriptions[1].binding = 0;
    _attributeDescriptions[1].location = 1; ///<< color
    _attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    _attributeDescriptions[1].offset = offsetof(Vertex, color);

    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_bindingDescriptions.size());
    _vertexInputInfo.pVertexBindingDescriptions = _bindingDescriptions.data();
    _vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_attributeDescriptions.size());
    _vertexInputInfo.pVertexAttributeDescriptions = _attributeDescriptions.data();

    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_input_assembly_state() noexcept
{
    _inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_viewport_state() noexcept
{
    _viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportInfo.viewportCount = 1;
    _viewportInfo.scissorCount = 1;
    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_rasterization_state() noexcept
{
    _rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizationInfo.depthClampEnable = VK_FALSE;
    _rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    _rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterizationInfo.lineWidth = 1.0f;
    _rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    _rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    _rasterizationInfo.depthBiasEnable = VK_FALSE;
    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_multisample_state() noexcept
{
    _multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisampleInfo.sampleShadingEnable = VK_FALSE;
    _multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_color_blend_state() noexcept
{
    _colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;

    _colorBlendAttachment.blendEnable = VK_FALSE;

    _colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _colorBlendInfo.logicOpEnable = VK_FALSE;
    _colorBlendInfo.attachmentCount = 1;
    _colorBlendInfo.pAttachments = &_colorBlendAttachment;
    return *this;
}

lumen::PipelineBuilder &lumen::PipelineBuilder::set_dynamic_state(const std::vector<VkDynamicState> &states) noexcept
{
    _dynamicStateEnables = states;
    _dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    _dynamicStateInfo.pDynamicStates = _dynamicStateEnables.data();
    _dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(_dynamicStateEnables.size());
    return *this;
}

VkPipeline lumen::PipelineBuilder::build(VkRenderPass render_pass, VkPipelineLayout *out_pipeline_layout)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vk_check(vkCreatePipelineLayout(_device.logicalDevice(), &pipelineLayoutInfo, nullptr, out_pipeline_layout), "failed to create pipeline layout!");

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(_shaderStages.size());
    pipelineInfo.pStages = _shaderStages.data();
    pipelineInfo.pVertexInputState = &_vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &_inputAssemblyInfo;
    pipelineInfo.pViewportState = &_viewportInfo;
    pipelineInfo.pRasterizationState = &_rasterizationInfo;
    pipelineInfo.pMultisampleState = &_multisampleInfo;
    pipelineInfo.pColorBlendState = &_colorBlendInfo;
    pipelineInfo.pDynamicState = &_dynamicStateInfo;
    pipelineInfo.layout = *out_pipeline_layout;
    pipelineInfo.renderPass = render_pass;
    pipelineInfo.subpass = 0;

    VkPipeline graphicsPipeline;
    vk_check(vkCreateGraphicsPipelines(_device.logicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline), "failed to create graphics pipeline!");

    return graphicsPipeline;
}
