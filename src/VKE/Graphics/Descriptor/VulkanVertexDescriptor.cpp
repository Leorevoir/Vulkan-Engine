#include <VKE/Graphics/Descriptor/VulkanVertexDescriptor.hpp>
#include <VKE/Maths/Vertex.hpp>

/**
* public
*/

vke::priv::VulkanVertexDescriptor::VulkanVertexDescriptor()
{
    _generate_descriptions();
}

VkPipelineVertexInputStateCreateInfo &vke::priv::VulkanVertexDescriptor::getState()
{
    return _input_state;
}

/**
* private
*/

void vke::priv::VulkanVertexDescriptor::_generate_descriptions()
{
    _input_binding.resize(1);
    _input_binding[0] = {.binding = 0, .stride = sizeof(maths::VertexV4), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX};

    _input_attributes.resize(3);
    _input_attributes[0] = {.location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(maths::VertexV4, _pos)};
    _input_attributes[0] = {.location = 1, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(maths::VertexV4, _uv)};
    _input_attributes[0] = {.location = 2, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(maths::VertexV4, _normal)};

    _input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _input_state.vertexBindingDescriptionCount = static_cast<u32>(_input_binding.size());
    _input_state.pVertexBindingDescriptions = _input_binding.data();
    _input_state.vertexAttributeDescriptionCount = static_cast<u32>(_input_attributes.size());
    _input_state.pVertexAttributeDescriptions = _input_attributes.data();
}
