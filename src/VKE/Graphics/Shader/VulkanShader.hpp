#pragma once

#include <VKE/Graphics/Shader/Base/VulkanShaderBase.hpp>

namespace vke {

class VKE_API VulkanShader : public priv::VulkanShaderBase
{
    public:
        explicit VulkanShader(const std::string &vertex_path, const std::string &fragment_path);
        ~VulkanShader() override = default;

        void initialize() override;

    private:
        std::string _vertex_path;
        std::string _fragment_path;
};

}// namespace vke
