#include <VKE/Error.hpp>
#include <VKE/Graphics/Shader/Base/VulkanShaderBase.hpp>

#include <fstream>
#include <iostream>

/**
* public
*/

vke::priv::VulkanShaderBase::~VulkanShaderBase()
{
    VKE_SAFE_CLEAN(_pipeline, vkDestroyPipeline(_context->getDevice(), _pipeline, VKE_NULLPTR));

    for (auto &shader : _shader_modules) {
        VKE_SAFE_CLEAN(shader, vkDestroyShaderModule(_context->getDevice(), shader, VKE_NULLPTR));
    }
}

void vke::priv::VulkanShaderBase::update()
{
    /* __update__ */
}

/**
 * helpers
 */

static VkShaderModule _load_shader(const char *filename, VkDevice device)
{
    std::ifstream stream(filename, std::ios::binary | std::ios::ate);

    if (!stream.is_open()) {
        throw vke::exception::RuntimeError("VulkanShader", "Could not open shader file: ", filename);
    }

    const std::streamsize size = stream.tellg();

    if (size <= 0) {
        throw vke::exception::RuntimeError("VulkanShader", "Shader file empty or invalid: ", filename);
    }

    stream.seekg(0, std::ios::beg);

    if (size % 4 != 0) {
        throw vke::exception::RuntimeError("VulkanShader", "SPIR-V file size not multiple of 4: ", filename);
    }

    std::vector<u32> code(static_cast<u32>(size / 4));

    if (!stream.read(reinterpret_cast<char *>(code.data()), size)) {
        throw vke::exception::RuntimeError("VulkanShader", "Failed to read full shader file: ", filename);
    }
    stream.close();

    VkShaderModuleCreateInfo module_create_info{};
    module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    module_create_info.codeSize = static_cast<u32>(size);
    module_create_info.pCode = code.data();

    VkShaderModule shader_module;
    const VkResult res = vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module);

    if (res != VK_SUCCESS) {
        throw vke::exception::RuntimeError("VulkanShader", "Failed to create shader module: ", filename);
    }

    std::cout << "shader loaded: " << filename << " (" << size << " bytes)" << std::endl;

    return shader_module;
}

/**
 * getters
 */

VkPipeline &vke::priv::VulkanShaderBase::getPipeline()
{
    return _pipeline;
}

const std::vector<VkPipelineShaderStageCreateInfo> &vke::priv::VulkanShaderBase::getShaderStages() const
{
    return _shader_stages;
}

VkCullModeFlags vke::priv::VulkanShaderBase::getCullMode() const
{
    return _cull_flags;
}

VkFrontFace vke::priv::VulkanShaderBase::getFrontFace() const
{
    return _front_face;
}

VkPipelineVertexInputStateCreateInfo vke::priv::VulkanShaderBase::getVertexInputState() const
{
    return _vertex_input_state;
}

bool vke::priv::VulkanShaderBase::isDepthBiasEnabled() const
{
    return _depth_bias_enabled;
}

bool vke::priv::VulkanShaderBase::isInstanceShader() const
{
    return _instance_shader;
}

bool vke::priv::VulkanShaderBase::isOneStage() const
{
    return _one_stage;
}

/**
 * setters
 */

void vke::priv::VulkanShaderBase::setCullMode(const VkCullModeFlags &flag)
{
    _cull_flags = flag;
}

void vke::priv::VulkanShaderBase::setFrontFace(const VkFrontFace &face)
{
    _front_face = face;
}

void vke::priv::VulkanShaderBase::setVertexInputState(const VkPipelineVertexInputStateCreateInfo &vertex_input_state)
{
    _vertex_input_state = vertex_input_state;
    _instance_shader = true;
}

void vke::priv::VulkanShaderBase::setDepthBiasEnabled(bool enabled)
{
    _depth_bias_enabled = enabled;
}

void vke::priv::VulkanShaderBase::setOneStage(bool one_stage)
{
    _one_stage = one_stage;
}

/**
 * protected
 */

VkPipelineShaderStageCreateInfo vke::priv::VulkanShaderBase::_load(const std::string &filename, const VkShaderStageFlagBits &stage)
{
    if (_context == nullptr) {
        throw vke::exception::RuntimeError("VulkanShader", "Context is not set. Call setContext() before loading shaders.");
    }

    VkPipelineShaderStageCreateInfo shader_stage_info = {};

    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.stage = stage;
    shader_stage_info.module = _load_shader(filename.c_str(), _context->getDevice());
    shader_stage_info.pName = "main";
    assert(shader_stage_info.module != VKE_NULLPTR);
    _shader_modules.push_back(shader_stage_info.module);
    return shader_stage_info;
}

/**
* private
*/
