#include <VKE/Error.hpp>
#include <VKE/Graphics/Shader/VulkanShader.hpp>

#include <fstream>

/**
* public
*/

vke::VulkanShader::VulkanShader(VulkanContext *context) : VulkanObject(context)
{
    /* __ctor__ */
}

vke::VulkanShader::~VulkanShader()
{
    VKE_SAFE_CLEAN(_pipeline, vkDestroyPipeline(_context->getDevice(), _pipeline, VKE_NULL_PTR));

    for (auto &shader : _shader_modules) {
        VKE_SAFE_CLEAN(shader, vkDestroyShaderModule(_context->getDevice(), shader, VKE_NULL_PTR));
    }
}

void vke::VulkanShader::update()
{
    /* __update__ */
}

/**
 * helpers
 */

static inline VkShaderModule _load_shader(const char *filename, VkDevice device)
{
    std::ifstream stream(filename, std::ios::binary | std::ios::in | std::ios::ate);

    if (!stream.is_open()) {
        throw vke::exception::RuntimeError("VulkanShader", "Could not open shader file: ", filename);
    }
    const u32 size = static_cast<u32>(stream.tellg());

    if (size % 4 != 0) {
        throw vke::exception::RuntimeError("VulkanShader", "SPIR-V file size is not a multiple of 4 bytes: ", filename);
    }

    stream.seekg(0, std::ios::beg);
    stream.seekg(0, std::ios::beg);
    std::vector<u32> code(size / 4);
    stream.read(reinterpret_cast<char *>(code.data()), size);
    stream.close();

    VkShaderModule shader_module = {};
    VkShaderModuleCreateInfo module_create_info{};

    module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    module_create_info.codeSize = code.size();
    module_create_info.pCode = code.data();

    VKE_ASSERT(vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module));
    return shader_module;
}

/**
 * getters
 */

VkPipeline &vke::VulkanShader::getPipeline()
{
    return _pipeline;
}

std::vector<VkPipelineShaderStageCreateInfo> &vke::VulkanShader::getShaderStages()
{
    return _shader_stages;
}

VkCullModeFlags vke::VulkanShader::getCullMode() const
{
    return _cull_flags;
}

VkFrontFace vke::VulkanShader::getFrontFace() const
{
    return _front_face;
}

VkPipelineVertexInputStateCreateInfo vke::VulkanShader::getVertexInputState() const
{
    return _vertex_input_state;
}

bool vke::VulkanShader::isDepthBiasEnabled() const
{
    return _depth_bias_enabled;
}

bool vke::VulkanShader::isInstanceShader() const
{
    return _instance_shader;
}

bool vke::VulkanShader::isOneStage() const
{
    return _one_stage;
}

/**
 * setters
 */

void vke::VulkanShader::setCullMode(const VkCullModeFlags &flag)
{
    _cull_flags = flag;
}

void vke::VulkanShader::setFrontFace(const VkFrontFace &face)
{
    _front_face = face;
}

void vke::VulkanShader::setVertexInputState(const VkPipelineVertexInputStateCreateInfo &vertex_input_state)
{
    _vertex_input_state = vertex_input_state;
    _instance_shader = true;
}

void vke::VulkanShader::setDepthBiasEnabled(bool enabled)
{
    _depth_bias_enabled = enabled;
}

void vke::VulkanShader::setOneStage(bool one_stage)
{
    _one_stage = one_stage;
}

/**
 * protected
 */

VkPipelineShaderStageCreateInfo vke::VulkanShader::_load(const std::string &filename, const VkShaderStageFlagBits &stage)
{
    VkPipelineShaderStageCreateInfo shader_stage_info = {};

    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.stage = stage;
    shader_stage_info.module = _load_shader(filename.c_str(), _context->getDevice());
    shader_stage_info.pName = "main";
    assert(shader_stage_info.module != VKE_NULL_PTR);
    _shader_modules.push_back(shader_stage_info.module);
    return shader_stage_info;
}

/**
* private
*/
