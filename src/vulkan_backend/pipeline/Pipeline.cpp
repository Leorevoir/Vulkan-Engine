#include "vulkan_backend/pipeline/Pipeline.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/pipeline/RenderPass.hpp"
#include "vulkan_backend/pipeline/Shader.hpp"
#include "vulkan_backend/utils/Result.hpp"
#include <vulkan/vulkan_core.h>

/**
 * static helpers
 */
// clang-format off

namespace {

static inline VkPipelineShaderStageCreateInfo __create_shader_stage(VkShaderModule module, VkShaderStageFlagBits stage) noexcept
{
    VkPipelineShaderStageCreateInfo shaderStageInfo{};

    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = module;
    shaderStageInfo.pName = "main";
    return shaderStageInfo;
}

static inline VkPipelineVertexInputStateCreateInfo __create_vertex_input_state() noexcept
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    return vertexInputInfo;
}

static inline VkPipelineInputAssemblyStateCreateInfo __create_input_assembly() noexcept
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return inputAssembly;
}

static inline VkPipelineViewportStateCreateInfo __create_viewport_state() noexcept
{
    VkPipelineViewportStateCreateInfo viewportState{};

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    return viewportState;
}

static inline VkPipelineRasterizationStateCreateInfo __create_rasterization_state() noexcept
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};

    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    return rasterizer;
}

static inline VkPipelineMultisampleStateCreateInfo __create_multisample_state() noexcept
{
    VkPipelineMultisampleStateCreateInfo multisampling{};

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    return multisampling;
}

static inline VkPipelineColorBlendStateCreateInfo __create_color_blending(
    const VkPipelineColorBlendAttachmentState &colorBlendAttachment
) noexcept
{
    VkPipelineColorBlendStateCreateInfo colorBlending{};

    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    return colorBlending;
}

static inline VkPipelineDynamicStateCreateInfo __create_dynamic_state() noexcept
{
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    return dynamicState;
}

static inline VkPipelineLayout __create_pipeline_layout(VkDevice device)
{
    VkPipelineLayout pipelineLayout{};
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vk_check(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout), "failed to create pipeline layout!");
    return pipelineLayout;
}

}// namespace

/**
* public
*/

lumen::Pipeline::Pipeline(Device &device, const RenderPass &renderPass, const std::string &vertShaderPath, const std::string &fragShaderPath) : _device(device)
{
    Shader vertShader(device, vertShaderPath);
    Shader fragShader(device, fragShaderPath);

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        __create_shader_stage(vertShader.handle(), VK_SHADER_STAGE_VERTEX_BIT),
        __create_shader_stage(fragShader.handle(), VK_SHADER_STAGE_FRAGMENT_BIT)
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = __create_vertex_input_state();
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = __create_input_assembly();
    VkPipelineViewportStateCreateInfo viewportState = __create_viewport_state();
    VkPipelineRasterizationStateCreateInfo rasterizer = __create_rasterization_state();
    VkPipelineMultisampleStateCreateInfo multisampling = __create_multisample_state();

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

   VkPipelineColorBlendStateCreateInfo colorBlending = __create_color_blending(colorBlendAttachment);
    VkPipelineDynamicStateCreateInfo dynamicState = __create_dynamic_state();
    _pipelineLayout = __create_pipeline_layout(device.logicalDevice());    VkGraphicsPipelineCreateInfo pipelineInfo{};

    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = renderPass.handle();
    pipelineInfo.subpass = 0;

    vk_check(vkCreateGraphicsPipelines(device.logicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline), "failed to create graphics pipeline!");
}

lumen::Pipeline::~Pipeline()
{
    vkDestroyPipeline(_device.logicalDevice(), _graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(_device.logicalDevice(), _pipelineLayout, nullptr);
}

/**
* getters
*/

VkPipeline lumen::Pipeline::handle() const noexcept
{
    return _graphicsPipeline;
}

VkPipelineLayout lumen::Pipeline::getLayout() const noexcept
{
    return _pipelineLayout;
}
