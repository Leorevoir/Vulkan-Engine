#include <utils/maths/Vertex.hpp>

#include <algorithm>

/**
* public
*/
// clang-format off

/**
* Vertex
*/

bool lumen::Vertex::operator==(const Vertex &other) const noexcept
{
    return (
        position == other.position
        && color == other.color
        && texCoord == other.texCoord
        && normal == other.normal
        && tangent == other.tangent
        && bitangent == other.bitangent
    );
}

/**
* AnimVertex
*/

void lumen::AnimVertex::addBoneData(const u32 boneInfoIndex, f32 weight) noexcept
{
    if (weight < 0.f || weight > 1.f) {
        weight = std::clamp(weight, 0.f, 1.f);
    }

    if (weight > 0.f) {

        for (u32 i = 0; i < 4; ++i) {

            if (boneWeights[i] == 0.f) {
                boneInfoIndices[i] = boneInfoIndex;
                boneWeights[i] = weight;
                return;
            }

        }

    }
}

void lumen::AnimVertex::normalizeBoneWeights() noexcept
{
    f32 totalWeight = 0.f;

    for (u32 i = 0; i < 4; ++i) {
        totalWeight += boneWeights[i];
    }

    if (totalWeight > 0.f) {

        for (u32 i = 0; i < 4; ++i) {
            boneWeights[i] /= totalWeight;
        }

    }
}
