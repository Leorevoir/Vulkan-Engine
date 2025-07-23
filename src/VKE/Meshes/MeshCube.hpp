#pragma once

#include <VKE/Maths/Vertex.hpp>
#include <VKE/Meshes/MeshObject.hpp>

namespace vke {

class VKE_API MeshCube : public MeshObject
{
    public:
        constexpr MeshCube() = default;
        ~MeshCube() override = default;

        void initialize() override;
        void update() override {};

    private:
        static constexpr u32 mesh_size = 36;
        std::array<maths::VertexV4, mesh_size> _vertices = {{
            {{_scale.x, _scale.y, _scale.z}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-_scale.x, -_scale.y, _scale.z}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-_scale.x, _scale.y, _scale.z}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{_scale.x, _scale.y, _scale.z}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{_scale.x, -_scale.y, _scale.z}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-_scale.x, -_scale.y, _scale.z}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

            {{_scale.x, _scale.y, -_scale.z}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-_scale.x, _scale.y, -_scale.z}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{_scale.x, _scale.y, -_scale.z}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{_scale.x, -_scale.y, -_scale.z}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

            {{_scale.x, _scale.y, _scale.z}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{_scale.x, -_scale.y, -_scale.z}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{_scale.x, -_scale.y, _scale.z}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{_scale.x, _scale.y, _scale.z}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{_scale.x, _scale.y, -_scale.z}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{_scale.x, -_scale.y, -_scale.z}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

            {{-_scale.x, _scale.y, _scale.z}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-_scale.x, -_scale.y, _scale.z}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-_scale.x, _scale.y, _scale.z}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-_scale.x, _scale.y, -_scale.z}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

            {{_scale.x, _scale.y, _scale.z}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-_scale.x, _scale.y, _scale.z}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-_scale.x, _scale.y, -_scale.z}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{_scale.x, _scale.y, _scale.z}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-_scale.x, _scale.y, -_scale.z}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{_scale.x, _scale.y, -_scale.z}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

            {{_scale.x, -_scale.y, _scale.z}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{-_scale.x, -_scale.y, _scale.z}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{_scale.x, -_scale.y, _scale.z}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{_scale.x, -_scale.y, -_scale.z}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{-_scale.x, -_scale.y, -_scale.z}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        }};

        const std::array<u32, mesh_size> indices = {
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35}};
};

}// namespace vke
