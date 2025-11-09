#include "TriangleApplication.hpp"

#include <utils/Filepath.hpp>
#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/renderer/Renderer.hpp>

/**
* protected
*/
// clang-format off

void TriangleApplication::on_create()
{
    const std::string vert_shader_path = Filepath::get_absolute("examples/Triangle/assets/shaders/triangle.vert.spv").string();
    const std::string frag_shader_path = Filepath::get_absolute("examples/Triangle/assets/shaders/triangle.frag.spv").string();

    _pipeline = std::make_unique<lumen::Pipeline>(
        get_context().get_device(),
        get_renderer().get_render_pass(),
        vert_shader_path, frag_shader_path)
    ;
}

void TriangleApplication::on_update()
{
    get_renderer().draw_frame(*_pipeline);
}

void TriangleApplication::on_destroy()
{
    _pipeline.reset();
}
