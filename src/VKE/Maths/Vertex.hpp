#pragma once

#include <VKE/Maths/Vector.hpp>

namespace vke {

namespace maths {

struct VKEM_API Vertex {
        Vector3f _pos;
        Vector2f _uv;
        Vector3f _normal;
};

struct VKEM_API VertexV3 {
        Vector3f _pos;
        Vector3f _uv;
        Vector3f _normal;
};

struct VKEM_API VertexV4 {
        Vector3f _pos;
        Vector4f _uv;
        Vector3f _normal;
};

}// namespace maths

}// namespace vke
