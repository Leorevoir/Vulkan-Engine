#include <utils/Filepath.hpp>

#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/pipeline/Shader.hpp>
#include <vulkan_backend/utils/Result.hpp>
#include <vulkan_backend/utils/VulkanUtils.hpp>

/**
 * static private
 */

namespace {

static inline VkShaderModule __create_shader_module_handle(lumen::Device &device, const std::string &filepath)
{
    const auto shader_code = Filepath::read(filepath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shader_code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shader_code.data());

    VkShaderModule shader_module_handle;
    vk_check(vkCreateShaderModule(device.logicalDevice(), &createInfo, nullptr, &shader_module_handle), "failed to create shader module!");
    return shader_module_handle;
}
}// namespace

/**
 * public
 */

lumen::Shader::Shader(Device &device, const std::string &filepath) : VulkanObject(device, __create_shader_module_handle(device, filepath))
{
    /* __ctor__ */
}

lumen::Shader::~Shader() noexcept
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(_device.logicalDevice(), _handle, nullptr);
    }
}
