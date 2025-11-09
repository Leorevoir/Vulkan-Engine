#include "vulkan_backend/pipeline/Pipeline.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/pipeline/PipelineBuilder.hpp"
#include "vulkan_backend/pipeline/RenderPass.hpp"

/**
 * public
 */
// clang-format off

lumen::Pipeline::Pipeline(Device &device, const RenderPass &renderPass, const std::string &vertShaderPath, const std::string &fragShaderPath)
    : VulkanObject(device, VK_NULL_HANDLE),
      _pipelineLayout(VK_NULL_HANDLE)
{
    PipelineBuilder builder{device};

    _handle = builder.set_shader_stages(vertShaderPath, fragShaderPath)
                            .set_vertex_input_state()
                            .set_input_assembly_state()
                            .set_viewport_state()
                            .set_rasterization_state()
                            .set_multisample_state()
                            .set_color_blend_state()
                            .set_dynamic_state({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
                            .build(renderPass.handle(), &_pipelineLayout);
}

lumen::Pipeline::~Pipeline() noexcept
{
    if (_pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(_device.logicalDevice(), _pipelineLayout, nullptr);
    }

    if (_handle != VK_NULL_HANDLE) {
        vkDestroyPipeline(_device.logicalDevice(), _handle, nullptr);
    }
}

/**
* getters
*/

VkPipelineLayout lumen::Pipeline::getLayout() const noexcept
{
    return _pipelineLayout;
}
