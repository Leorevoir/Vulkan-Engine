#include "VKE/Filename.hpp"
#include <Example/TriangleEngine.hpp>
#include <VKE/Error.hpp>
#include <VKE/Maths/Vertex.hpp>

#include <iostream>

/**
 * Triangle
 */

void Triangle::initialize()
{
    const vke::maths::VertexV4 vertices[] = {
        {{0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.f, 0.f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.f, 0.f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.f, 0.f}, {0.0f, 0.0f, 1.0f}},
    };
    constexpr u32 vertice_size = (sizeof(vertices) / sizeof(vertices[0])) * sizeof(vke::maths::VertexV4);

    constexpr u32 indices[] = {0, 1, 2};
    constexpr u32 _index_size = static_cast<u32>(sizeof(indices) / sizeof(u32)) * sizeof(u32);

    _index_count = static_cast<u32>(sizeof(indices) / sizeof(u32));

    VKE_ASSERT(_context->_device->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &_vertex_buffer, vertice_size, vertices));
    VKE_ASSERT(_context->_device->createBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &_index_buffer, _index_size, indices));
}

void Triangle::update()
{
    /* __update__ */
}

/**
 * Shader
 */

void TriangleShader::initialize()
{
    if (_context == nullptr) {
        throw vke::exception::RuntimeError("TriangleShader", "Context is not set. Call setContext() before initializing shaders.");
    }

    const std::string vertex_shader_path = vke::Filename::getPath("assets/shaders/cache/triangle.vert.spv");
    const std::string fragment_shader_path = vke::Filename::getPath("assets/shaders/cache/triangle.frag.spv");

    if (!vke::Filename::exists(vertex_shader_path) || !vke::Filename::exists(fragment_shader_path)) {
        throw vke::exception::RuntimeError("Triangle::initialize()", "Shader files not found: ", vertex_shader_path, " or ", fragment_shader_path);
    }

    _shader_stages.push_back(_load(vertex_shader_path, VK_SHADER_STAGE_VERTEX_BIT));
    _shader_stages.push_back(_load(fragment_shader_path, VK_SHADER_STAGE_FRAGMENT_BIT));
    std::cout << "shader initialized and pushed" << std::endl;
}

/**
* protected
*/

void TriangleEngine::initialize()
{
    auto context = _context.get();

    _triangle = vke::VulkanObject::Create<Triangle>(context);
    _triangle->initialize();

    _triangle_shader = vke::VulkanObject::Create<TriangleShader>(context);
    _triangle_shader->initialize();

    _triangle_uniform = vke::VulkanObject::Create<vke::UniformCamera>(context);
    _triangle_uniform->initialize();

    _descriptor_set->add(0, 0, &_triangle_uniform->getUniformBuffer()._descriptorInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    _descriptor_set->generate(&_pipeline_layout);

    _pipelines->createBase(_pipeline_layout, _render_pass);
    _pipelines->create(_triangle_shader);
}

void TriangleEngine::update()
{
    _triangle_uniform->update();
}

void TriangleEngine::drawObjects(VkCommandBuffer &cmd)
{
    _triangle->bind(cmd, _triangle_shader);
}
