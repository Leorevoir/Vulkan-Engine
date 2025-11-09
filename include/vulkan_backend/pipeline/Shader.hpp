#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"
#include <string>

namespace lumen {

class Shader : public VulkanObject<VkShaderModule>
{
    public:
        Shader(Device &device, const std::string &filepath);
        ~Shader() noexcept override;
};

}// namespace lumen
