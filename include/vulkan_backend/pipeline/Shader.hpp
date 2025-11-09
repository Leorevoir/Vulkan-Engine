#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class Shader
{
    public:
        Shader(Device &device, const std::string &filepath);
        ~Shader() noexcept;

        VkShaderModule handle() const noexcept;

    private:
        VkShaderModule _shaderModule;
        Device &_device;
};

}// namespace lumen
