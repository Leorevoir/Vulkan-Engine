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
* {position}            {color}
*/
static constexpr lumen::Vertex g_vertices[] = {
    {{ 0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
};

/**
 * triangle indices
 */
static constexpr uint32_t g_indices[] = {
    0, 1, 2
};

}

/**
* protected
*/
// clang-format on

void TriangleApplication::on_create()
{
    _pipeline = std::make_unique<lumen::Pipeline>(get_context().get_device(), get_renderer().get_render_pass());

    auto triangle_mesh = std::make_shared<lumen::VulkanMesh>(get_context(), g_vertices, g_indices);
    const lumen::RenderObject triangle_object = {.mesh = triangle_mesh};

    _render_objects.push_back(triangle_object);
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
