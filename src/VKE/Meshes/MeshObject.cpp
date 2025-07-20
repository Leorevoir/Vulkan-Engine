#include <VKE/Error.hpp>
#include <VKE/Meshes/MeshObject.hpp>

/**
* public
*/

vke::MeshObject::~MeshObject()
{
    _vertex_buffer.destroy();
    _index_buffer.destroy();
}

void vke::MeshObject::bind(VkCommandBuffer &cmd_buffer, std::shared_ptr<VulkanShader> vulkan_shader)
{
    VkDeviceSize offsets[1] = {0};
    auto pipeline = vulkan_shader->getPipeline();

    if (pipeline) {
        vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    } else {
        throw exception::RuntimeError("MeshObject::build", "Pipeline is not set for the shader.");
    }

    vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &_vertex_buffer._buffer, offsets);
    vkCmdBindIndexBuffer(cmd_buffer, _index_buffer._buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd_buffer, _index_count, 1, 0, 0, 0);
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

/**
 * protected
 */

/**
* private
*/
