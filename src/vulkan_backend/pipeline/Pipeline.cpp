#include <utils/Filepath.hpp>
#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/pipeline/Pipeline.hpp>
#include <vulkan_backend/pipeline/PipelineBuilder.hpp>
#include <vulkan_backend/pipeline/RenderPass.hpp>

/**
 * public
 */
// clang-format off

lumen::Pipeline::Pipeline(Device &device, const RenderPass &render_pass, const std::string &vert_shader_path, const std::string &frag_shader_path)
    : VulkanObject(device, VK_NULL_HANDLE), _pipelineLayout(VK_NULL_HANDLE)
{
    _set_shader_stages(render_pass, vert_shader_path, frag_shader_path, false);
}

lumen::Pipeline::Pipeline(Device &device, const RenderPass &render_pass)
    : VulkanObject(device, VK_NULL_HANDLE), _pipelineLayout(VK_NULL_HANDLE)
{
    const std::string vert_shader_path = Filepath::get_absolute("assets/shaders/mesh.vert").string();
    const std::string frag_shader_path = Filepath::get_absolute("assets/shaders/mesh.frag").string();

    const std::string vert_shader_content = Filepath::read_str(vert_shader_path);
    const std::string frag_shader_content = Filepath::read_str(frag_shader_path);

    _set_shader_stages(render_pass, vert_shader_content, frag_shader_content, true);
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

/**
* private
*/
// clang-format off

void lumen::Pipeline::_set_shader_stages(
    const RenderPass &render_pass,
    const std::string &vert_shader_path,
    const std::string &frag_shader_path,
    bool is_source)
{
    PipelineBuilder builder{_device};

    builder.set_shader_stages(vert_shader_path, frag_shader_path, is_source)
        .set_vertex_input_state()
        .set_input_assembly_state()
        .set_viewport_state()
        .set_rasterization_state()
        .set_multisample_state()
        .set_color_blend_state()
        .set_dynamic_state({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
        .build(render_pass.handle(), &_pipelineLayout);
}
