#include <Example/CubeEngine.hpp>
#include <VKE/Filename.hpp>

/**
* public
*/

static const std::string vertex_path = vke::Filename::getPath("assets/shaders/cache/triangle.vert.spv");
static const std::string fragment_path = vke::Filename::getPath("assets/shaders/cache/triangle.frag.spv");

void CubeEngine::initialize()
{
    vke::VulkanContext *context = _context.get();

    _mesh = vke::VulkanObject::Create<vke::MeshCube>(context);
    _shader = vke::VulkanObject::Create<vke::VulkanShader>(context, vertex_path, fragment_path);
    _uniform = vke::VulkanObject::Create<vke::UniformCamera>(context);

    _descriptor_set->add(0, 0, &(_uniform->getBuffer()._descriptor), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    _descriptor_set->generate(&_pipeline_layout);

    _pipelines->createBase(_pipeline_layout, _render_pass);
    _pipelines->create(_shader);
}

void CubeEngine::update()
{
    _uniform->update();
}

void CubeEngine::drawObjects(VkCommandBuffer &cmd)
{
    _mesh->bind(cmd, _shader);
}
