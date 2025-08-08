#include <Example/CubeEngine.hpp>

/**
* public
*/

void CubeEngine::initialize()
{
    vke::VulkanContext *context = _context.get();

    _mesh = vke::VulkanObject::Create<vke::MeshCube>(context);

    _descriptor_set->generate(&_pipeline_layout);

    _pipelines->createBase(_pipeline_layout, _render_pass);
    _pipelines->create(_mesh->getShader());
}

void CubeEngine::update()
{
    _mesh->update();
}

void CubeEngine::drawObjects(VkCommandBuffer &cmd)
{
    _mesh->bind(cmd);
}
