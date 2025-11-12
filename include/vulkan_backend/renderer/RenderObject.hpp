#pragma once

#include <memory>

namespace lumen {

class VulkanMesh;

struct RenderObject {
        std::shared_ptr<VulkanMesh> mesh;
};

}// namespace lumen
