#pragma once

#include <utils/maths/Vertex.hpp>
#include <vulkan_backend/resources/Buffer.hpp>

#include <memory>
#include <vector>

namespace lumen {

class GraphicsContext;

class VulkanMesh
{
    public:
        VulkanMesh(GraphicsContext &context, const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

        template<size_t V, size_t I>
            requires(sizeof(lumen::Vertex) * V > 0 && sizeof(uint32_t) * I > 0)
        VulkanMesh(GraphicsContext &context, const Vertex (&vertices)[V], const uint32_t (&indices)[I]);

        ~VulkanMesh() = default;

        VulkanMesh(const VulkanMesh &) = delete;
        VulkanMesh &operator=(const VulkanMesh &) = delete;

        void bind(VkCommandBuffer command_buffer) const;
        void draw(VkCommandBuffer command_buffer) const;

    private:
        std::unique_ptr<Buffer> _vertex_buffer;
        std::unique_ptr<Buffer> _index_buffer;
        uint32_t _index_count;
};

}// namespace lumen

#include "inline/VulkanMesh.inl"
