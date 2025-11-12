#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/core/VulkanObject.hpp>

#include <string>

namespace lumen {

class Shader : public VulkanObject<VkShaderModule>
{
    public:
        Shader(Device &device, const std::string &filepath);
        Shader(Device &device, const std::string &source, VkShaderStageFlagBits stage);
        ~Shader() noexcept override;
};

}// namespace lumen
