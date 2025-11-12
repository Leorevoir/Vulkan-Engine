#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/resources/VulkanMesh.hpp>

/**
* public
*/
// clang-format off

lumen::VulkanMesh::VulkanMesh(GraphicsContext &context, const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : _index_count(static_cast<uint32_t>(indices.size()))
{
    const VkDeviceSize vertex_buffer_size = sizeof(vertices[0]) * vertices.size();
    const VkDeviceSize index_buffer_size = sizeof(indices[0]) * indices.size();

    _vertex_buffer = std::make_unique<Buffer>(context, vertex_buffer_size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    _vertex_buffer->write(vertices.data(), vertex_buffer_size);

    _index_buffer = std::make_unique<Buffer>(context, index_buffer_size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    _index_buffer->write(indices.data(), index_buffer_size);
}

void lumen::VulkanMesh::bind(VkCommandBuffer command_buffer) const
{
    const VkBuffer vertex_buffers[] = {_vertex_buffer->handle()};
    const VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, _index_buffer->handle(), 0, VK_INDEX_TYPE_UINT32);
}

void lumen::VulkanMesh::draw(VkCommandBuffer command_buffer) const
{
    vkCmdDrawIndexed(command_buffer, _index_count, 1, 0, 0, 0);
}
