#pragma once

#include <utils/maths/Vertex.hpp>

/**
* public
*/
// clang-format off

constexpr lumen::Vertex::Vertex(
    const Vec3f &position_,
    const Vec3f &color_,
    const Vec3f &texCoord_,
    const Vec3f &normal_,
    const Vec3f &tangent_,
    const Vec3f &bitangent_
) noexcept
{
    this->position = position_;
    this->color = color_;
    this->texCoord = texCoord_;
    this->normal = normal_;
    this->tangent = tangent_;
    this->bitangent = bitangent_;
}
