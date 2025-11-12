#pragma once

#include <vulkan_backend/resources/VulkanMesh.hpp>

template<size_t V, size_t I>
    requires(sizeof(lumen::Vertex) * V > 0 && sizeof(uint32_t) * I > 0)
lumen::VulkanMesh::VulkanMesh(GraphicsContext &context, const Vertex (&vertices)[V], const uint32_t (&indices)[I])
    : _index_count(static_cast<uint32_t>(I))
{
    const VkDeviceSize vertex_buffer_size = sizeof(Vertex) * V;
    const VkDeviceSize index_buffer_size = sizeof(uint32_t) * I;

    _vertex_buffer = std::make_unique<Buffer>(context, vertex_buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    _vertex_buffer->write(vertices, vertex_buffer_size);

    _index_buffer = std::make_unique<Buffer>(context, index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    _index_buffer->write(indices, index_buffer_size);
}
