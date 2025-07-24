#include <VKE/Error.hpp>
#include <VKE/Filename.hpp>

#include <VKE/Meshes/MeshCube.hpp>

#include <VKE/Graphics/Shader/UniformCamera.hpp>
#include <VKE/Graphics/Shader/VulkanShader.hpp>

/**
 * static
 */

static const std::string vertex_path = vke::Filename::getPath("assets/shaders/cache/mesh.vert.spv");
static const std::string fragment_path = vke::Filename::getPath("assets/shaders/cache/mesh.frag.spv");

/*
* public
*/

void vke::MeshCube::initialize()
{
    _shader = VulkanObject::Create<VulkanShader>(_context, vertex_path, fragment_path);
    _uniform = VulkanObject::Create<UniformCamera>(_context);
    _index_count = mesh_size;

    constexpr u32 vertices_size = sizeof(maths::VertexV4) * mesh_size;
    constexpr u32 indices_size = sizeof(u32) * mesh_size;
    constexpr u32 color_size = sizeof(maths::Color) * mesh_size;

    for (u32 i = 0; i < mesh_size; ++i) {
        _colors[i] = _color;
    }

    auto *device = _context->getDevice();

    VKE_ASSERT(device->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_vertex_buffer,
        vertices_size, _vertices.data()));

    VKE_ASSERT(device->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_color_buffer,
        color_size, _colors.data()));

    VKE_ASSERT(device->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &_index_buffer,
        indices_size, _indices.data()));

    auto descriptor = _context->getDescriptorSet();

    descriptor->add(0, 0, &(_uniform->getBuffer()._descriptor), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
}

void vke::MeshCube::update()
{
    _uniform->update();
}

/**
 * getters
 */

std::shared_ptr<vke::VulkanShader> vke::MeshCube::getShader() const
{
    return _shader;
}

/*
* private
*/
