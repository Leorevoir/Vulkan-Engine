#pragma once

#include <VKE/Maths/Base.hpp>
#include <VKE/Types.hpp>

namespace vke {
namespace maths {

static inline constexpr VKEM_API f32 radians(const f32 degrees)
{
    return VKEM_DEG2RAD(degrees);
}

}// namespace maths
}// namespace vke
