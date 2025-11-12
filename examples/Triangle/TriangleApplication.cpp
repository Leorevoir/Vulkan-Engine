#include "TriangleApplication.hpp"

#include <utils/Filepath.hpp>
#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/renderer/Renderer.hpp>
#include <vulkan_backend/resources/VulkanMesh.hpp>

/**
 * static helpers
 */
// clang-format off

namespace {

/**
* {position}    {color}
*/
static constexpr lumen::Vertex g_vertices[] = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
};

/**
 * [first triangle]   [second triangle]
 */
static constexpr uint32_t g_indices[] = {
    0, 1, 2,
    2, 3, 0
};

}

/**
* protected
*/
// clang-format on

void TriangleApplication::on_create()
{
    const std::string vert_shader_path = Filepath::get_absolute("examples/Triangle/assets/shaders/triangle.vert.spv").string();
    const std::string frag_shader_path = Filepath::get_absolute("examples/Triangle/assets/shaders/triangle.frag.spv").string();

    _pipeline =
        std::make_unique<lumen::Pipeline>(get_context().get_device(), get_renderer().get_render_pass(), vert_shader_path, frag_shader_path);

    auto square_mesh = std::make_shared<lumen::VulkanMesh>(get_context(), g_vertices, g_indices);
    const lumen::RenderObject square_object = {.mesh = square_mesh};

    _render_objects.push_back(square_object);
}

void TriangleApplication::on_update()
{
    get_renderer().draw_frame(_render_objects, *_pipeline);
}

void TriangleApplication::on_destroy()
{
    _render_objects.clear();
    _pipeline.reset();
}
