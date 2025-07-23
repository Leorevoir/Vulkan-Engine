#include <VKE/Error.hpp>
#include <VKE/Meshes/MeshCube.hpp>

#include <iostream>

/*
* public
*/

void vke::MeshCube::initialize()
{
    _index_count = mesh_size;

    constexpr u32 vertices_size = sizeof(maths::VertexV4) * mesh_size;
    constexpr u32 indices_size = sizeof(u32) * mesh_size;

    VKE_ASSERT(_context->_device->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &_vertex_buffer, vertices_size, _vertices.data()));
    VKE_ASSERT(_context->_device->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &_index_buffer, indices_size, indices.data()));
}

/*
* private
*/
