#pragma once

#include <utils/maths/Vertex.hpp>

namespace lumen {

namespace details {

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wpedantic"
#endif

struct TriangleData {

        union {

                struct {
                        Vertex v0, v1, v2;
                };
                Vertex vertices[3]{};
        };
};

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(__clang__)
    #pragma clang diagnostic pop
#endif

}// namespace details

struct Triangle : public details::TriangleData {
        Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2) noexcept;
        Triangle() = default;
};

}// namespace lumen
