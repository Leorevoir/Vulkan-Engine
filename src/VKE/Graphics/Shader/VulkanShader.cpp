#include <VKE/Graphics/Shader/VulkanShader.hpp>

/**
* public
*/

vke::VulkanShader::VulkanShader(const std::string &vertex_path, const std::string &fragment_path) : _vertex_path(vertex_path), _fragment_path(fragment_path)
{
    _shader_stages.resize(2);
}

void vke::VulkanShader::initialize()
{
    _shader_stages[0] = _load(_vertex_path, VK_SHADER_STAGE_VERTEX_BIT);
    _shader_stages[1] = _load(_fragment_path, VK_SHADER_STAGE_FRAGMENT_BIT);
}

/**
* protected
*/
