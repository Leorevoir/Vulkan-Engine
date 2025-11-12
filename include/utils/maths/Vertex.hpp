#pragma once

#include <utils/maths/Vec.hpp>

// clang-format off

namespace lumen {

namespace details {

struct VertexData {

        Vec3f position{};
        Vec3f color{};
        Vec3f texCoord{};
        Vec3f normal{};
        Vec3f tangent{};
        Vec3f bitangent{};
};

}// namespace details

struct Vertex : public details::VertexData {

        Vertex() = default;
        constexpr Vertex(
            const Vec3f &position_,
            const Vec3f &color_,
            const Vec3f &texCoord_ = {},
            const Vec3f &normal_ = {},
            const Vec3f &tangent_ = {},
            const Vec3f &bitangent_ = {}
        ) noexcept;
        bool operator==(const Vertex &other) const noexcept;
};

struct AnimVertex : public details::VertexData {

        u32 boneInfoIndices[4]{0, 0, 0, 0};
        f32 boneWeights[4]{0.f, 0.f, 0.f, 0.f};

        bool operator==(const AnimVertex &other) const noexcept;
        void addBoneData(const u32 boneInfoIndex, f32 weight) noexcept;
        void normalizeBoneWeights() noexcept;
};

}// namespace lumen

#include "inline/Vertex.inl"
