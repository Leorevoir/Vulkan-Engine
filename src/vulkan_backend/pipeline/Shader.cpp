#include <utils/Filepath.hpp>

#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/pipeline/Shader.hpp>
#include <vulkan_backend/utils/Result.hpp>
#include <vulkan_backend/utils/VulkanUtils.hpp>

#include <shaderc/shaderc.hpp>

/**
 * static private
 */

namespace {

static shaderc_shader_kind __get_shaderc_kind(VkShaderStageFlagBits stage)
{
    switch (stage) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return shaderc_glsl_vertex_shader;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return shaderc_glsl_fragment_shader;
        default:
            throw std::runtime_error("Unsupported shader stage");
    }
}

static std::vector<uint32_t> __compile_glsl_to_spirv(const std::string &source, VkShaderStageFlagBits stage)
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    const auto result = compiler.CompileGlslToSpv(source, __get_shaderc_kind(stage), "shader", options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        throw std::runtime_error("Shader compilation failed: " + result.GetErrorMessage());
    }

    return {result.begin(), result.end()};
}

static inline VkShaderModule __create_shader_module_handle(lumen::Device &device, const std::vector<uint32_t> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = code.data();

    VkShaderModule shader_module_handle;
    vk_check(vkCreateShaderModule(device.logicalDevice(), &createInfo, nullptr, &shader_module_handle), "failed to create shader module!");
    return shader_module_handle;
}

static inline VkShaderModule __create_shader_module_from_file(lumen::Device &device, const std::string &filepath)
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

lumen::Shader::Shader(Device &device, const std::string &filepath)
    : VulkanObject(device, __create_shader_module_from_file(device, filepath))
{
    /* __ctor__ */
}

lumen::Shader::Shader(Device &device, const std::string &source, VkShaderStageFlagBits stage)
    : VulkanObject(device, __create_shader_module_handle(device, __compile_glsl_to_spirv(source, stage)))
{
    /* __ctor__ */
}

lumen::Shader::~Shader() noexcept
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(_device.logicalDevice(), _handle, nullptr);
    }
}
