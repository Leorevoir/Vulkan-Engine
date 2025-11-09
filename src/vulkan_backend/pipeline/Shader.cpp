#include "vulkan_backend/pipeline/Shader.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"
#include "vulkan_backend/utils/VulkanUtils.hpp"

/**
 * public
 */

lumen::Shader::Shader(Device &device, const std::string &filepath) : _device(device)
{
    const auto shader_code = utils::read_file(filepath);
    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shader_code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shader_code.data());

    vk_check(vkCreateShaderModule(_device.logicalDevice(), &createInfo, nullptr, &_shaderModule), "failed to create shader module!");
}

lumen::Shader::~Shader() noexcept
{
    vkDestroyShaderModule(_device.logicalDevice(), _shaderModule, nullptr);
}

/**
* getters
*/

VkShaderModule lumen::Shader::handle() const noexcept
{
    return _shaderModule;
}
