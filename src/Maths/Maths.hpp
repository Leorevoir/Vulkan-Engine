#pragma once

#include <type_traits>

namespace vk::maths {

#define VK_MATHS_NOT_NULL(x) (x > 0)
#define VK_MATHS_ASSERT_NUMBERS(N, T) (std::is_arithmetic_v<T>)

#define VK_MATHS_BASE_TEMPLATE(N, T)                                                                                                                                     \
    using value_type = T;                                                                                                                                                \
    using size_type = u32;                                                                                                                                               \
    static constexpr size_type size = N;

}// namespace vk::maths
