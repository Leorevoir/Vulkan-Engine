#include <VKE/Error.hpp>
#include <VKE/Meshes/MeshObject.hpp>

#include <VKE/Graphics/Device/VulkanBuffer.hpp>
#include <VKE/Graphics/Shader/VulkanShader.hpp>

/**
* public
*/

vke::MeshObject::~MeshObject()
{
    _vertex_buffer.destroy();
    _index_buffer.destroy();
    _color_buffer.destroy();
}

void vke::MeshObject::bind(VkCommandBuffer &cmd_buffer, std::shared_ptr<VulkanShader> vulkan_shader)
{
    const auto pipeline = vulkan_shader->getPipeline();

    if (!pipeline) {
        throw exception::RuntimeError("MeshObject::build", "Pipeline is not set for the shader.");
    }

    vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    const VkBuffer vertex_buffers[] = {_vertex_buffer._buffer, _color_buffer._buffer};
    const VkDeviceSize offsets[] = {0, 0};

    vkCmdBindVertexBuffers(cmd_buffer, 0, 2, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(cmd_buffer, _index_buffer._buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd_buffer, _index_count, 1, 0, 0, 0);
}

void vke::MeshObject::bind(VkCommandBuffer &cmd_buffer)
{
    if (!_shader) {
        throw exception::RuntimeError("MeshObject::bind", "Shader is not set for the MeshObject.");
    }

    this->bind(cmd_buffer, _shader);
}

/**
 * getters
 */

const vke::maths::Vector3f &vke::MeshObject::getPosition() const
{
    return _position;
}

const vke::maths::Vector3f &vke::MeshObject::getRotation() const
{
    return _rotation;
}

const vke::maths::Vector3f &vke::MeshObject::getScale() const
{
    return _scale;
}

/**
 * setters
 */

void vke::MeshObject::setPosition(const vke::maths::Vector3f &position)
{
    _position = position;
}

void vke::MeshObject::setRotation(const vke::maths::Vector3f &rotation)
{
    _rotation = rotation;
}

void vke::MeshObject::setScale(const vke::maths::Vector3f &scale)
{
    _scale = scale;
}

void vke::MeshObject::setColor(const vke::maths::Vector4f &color)
{
    _color = color;
}

/**
 * protected
 */

/**
* private
*/
