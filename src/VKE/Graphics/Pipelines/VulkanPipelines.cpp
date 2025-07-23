#include <VKE/Error.hpp>
#include <VKE/Graphics/Pipelines/VulkanPipelines.hpp>
#include <VKE/Types.hpp>
#include <cassert>

/**
 * helpers
 */

static inline VkPipelineDynamicStateCreateInfo _create_dynamic_state(const VkDynamicState *dynamic_states, u32 dynamic_state_count,
    VkPipelineDynamicStateCreateFlags flags = 0)
{
    VkPipelineDynamicStateCreateInfo dynamic_state_info = {};

    dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_info.pDynamicStates = dynamic_states;
    dynamic_state_info.dynamicStateCount = dynamic_state_count;
    dynamic_state_info.flags = flags;
    return dynamic_state_info;
}

/**
* public
*/

vke::priv::VulkanPipelines::VulkanPipelines(VkDevice &device, VkPipelineVertexInputStateCreateInfo &state, VkPipelineCache cache)
    : _device(device), _input_state(state), _pipeline_cache(cache)
{
    /* __ctor__ */
}

void vke::priv::VulkanPipelines::createBase(const VkPipelineLayout &pipeline_layout, const VkRenderPass &render_pass)
{
    _input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _input_assembly_state.primitiveRestartEnable = VK_FALSE;

    _rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterization_state.cullMode = VK_CULL_MODE_NONE;
    _rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    _rasterization_state.depthClampEnable = VK_FALSE;
    _rasterization_state.lineWidth = 1.0f;

    _blend_attachment_state.blendEnable = VK_FALSE;
    _blend_attachment_state.colorWriteMask = 0xf;

    _color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _color_blend_state.attachmentCount = 1;
    _color_blend_state.pAttachments = &_blend_attachment_state;

    _depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    _depth_stencil_state.depthTestEnable = VK_TRUE;
    _depth_stencil_state.depthWriteEnable = VK_TRUE;
    _depth_stencil_state.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    _depth_stencil_state.back.compareOp = VK_COMPARE_OP_ALWAYS;

    _viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewport_state.viewportCount = 1;
    _viewport_state.scissorCount = 1;
    _viewport_state.pViewports = VKE_NULLPTR;
    _viewport_state.pScissors = VKE_NULLPTR;

    _multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    _multisample_state.sampleShadingEnable = VK_TRUE;
    _multisample_state.minSampleShading = 0.2f;

    _dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    _dynamic_state.pDynamicStates = _dynamic_state_enables.data();
    _dynamic_state.dynamicStateCount = static_cast<u32>(_dynamic_state_enables.size());

    _pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    _pipeline_info.layout = pipeline_layout;
    _pipeline_info.renderPass = render_pass;
    _pipeline_info.basePipelineIndex = -1;
    _pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

    _pipeline_info.pVertexInputState = &_input_state;
    _pipeline_info.pInputAssemblyState = &_input_assembly_state;
    _pipeline_info.pRasterizationState = &_rasterization_state;
    _pipeline_info.pColorBlendState = &_color_blend_state;
    _pipeline_info.pMultisampleState = &_multisample_state;
    _pipeline_info.pViewportState = &_viewport_state;
    _pipeline_info.pDepthStencilState = &_depth_stencil_state;
    _pipeline_info.pDynamicState = &_dynamic_state;
    _pipeline_info.stageCount = static_cast<uint32_t>(_shader_stages.size());
    _pipeline_info.pStages = _shader_stages.data();

    _blend_attachment_state.blendEnable = VK_TRUE;
    _blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    _blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    _blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    _blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    _blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    _blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
}

void vke::priv::VulkanPipelines::create(VulkanShader *shader, VkPolygonMode polygon_mode)
{
    assert(shader != VKE_NULLPTR && "VulkanPipelines::create: shader is null");
    if (shader->getPipeline()) {
        vkDestroyPipeline(_device, shader->getPipeline(), VKE_NULLPTR);
    }
    _rasterization_state.polygonMode = polygon_mode;
    _rasterization_state.cullMode = shader->getCullMode();
    _rasterization_state.frontFace = shader->getFrontFace();

    if (shader->isDepthBiasEnabled()) {
        _rasterization_state.depthBiasEnable = VK_TRUE;
        _dynamic_state_enables.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);
        _dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        _dynamic_state.pDynamicStates = _dynamic_state_enables.data();
        _dynamic_state.dynamicStateCount = static_cast<u32>(_dynamic_state_enables.size());
        _dynamic_state.flags = 0;
    }

    if (shader->isOneStage()) {
        _pipeline_info.stageCount = 1;
        _color_blend_state.attachmentCount = 0;
        _shader_stages[0] = shader->getShaderStages()[0];
    } else {
        _shader_stages[0] = shader->getShaderStages()[0];
        _shader_stages[1] = shader->getShaderStages()[1];
    }

    if (shader->isInstanceShader()) {
        _input_state = shader->getVertexInputState();
    }

    VKE_ASSERT(vkCreateGraphicsPipelines(_device, _pipeline_cache, 1, &_pipeline_info, VKE_NULLPTR, &(shader->getPipeline())));
}

void vke::priv::VulkanPipelines::create(VulkanShader *shader, VkRenderPass render_pass, VkPolygonMode polygon_mode)
{
    assert(shader != VKE_NULLPTR && "VulkanPipelines::create: shader is null");
    _pipeline_info.renderPass = render_pass;
    create(shader, polygon_mode);
}

void vke::priv::VulkanPipelines::create(std::shared_ptr<VulkanShader> shader, VkPolygonMode polygon_mode)
{
    assert(shader != VKE_NULLPTR && "VulkanPipelines::create: shader is null");
    create(shader.get(), polygon_mode);
}

void vke::priv::VulkanPipelines::create(std::shared_ptr<VulkanShader> shader, VkRenderPass render_pass, VkPolygonMode polygon_mode)
{
    assert(shader != VKE_NULLPTR && "VulkanPipelines::create: shader is null");
    create(shader.get(), render_pass, polygon_mode);
}

/**
* private
*/
