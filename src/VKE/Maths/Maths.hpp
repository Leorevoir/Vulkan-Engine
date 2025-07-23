#pragma once

#include <VKE/Maths/Base.hpp>
#include <VKE/Types.hpp>

namespace vke {
namespace maths {

static inline constexpr VKEM_API f32 radians(const f32 degrees)
{
    return VKEM_DEG2RAD(degrees);
}

template<typename T>
static inline constexpr VKEM_API T lerp(const T a, T b, const T t)
{
    return a + t * (b - a);
}

template<typename T>
static inline constexpr VKEM_API T clamp(const T value, const T min_value, const T max_value)
{
    return (value < min_value) ? min_value : ((value > max_value) ? max_value : value);
}

}// namespace maths
}// namespace vke
