#pragma once

#include <type_traits>//<< std::is_arithmetic_v

#define VKM_API

#define VK_MATHS_NOT_NULL(x) (x > 0)
#define VK_MATHS_ASSERT_NUMBERS(T) (std::is_arithmetic_v<T>)
#define VK_MATHS_ASSERT_CONVERTIBLE(U, T) (std::convertible_to<U, T>)

#define VK_MATHS_ASSERT_VECTOR_SIZE(N) static_assert(N >= 1 && N <= 4, "Vector size must be between 1 and 4");

#define VK_MATHS_BASE_TEMPLATE(T)                                                                                                                                        \
    using value_type = T;                                                                                                                                                \
    using size_type = u32;

#define VK_MATHS_VEC_TEMPLATE(N, T)                                                                                                                                      \
    VK_MATHS_BASE_TEMPLATE(T)                                                                                                                                            \
    static constexpr size_type size = N;                                                                                                                                 \
    union {                                                                                                                                                              \
            T _data[N];                                                                                                                                                  \
            detail::VectorData<N, T> _named;                                                                                                                             \
    };

#define VK_MATHS_MAT_TEMPLATE(N, M, T)                                                                                                                                   \
    VK_MATHS_BASE_TEMPLATE(T)                                                                                                                                            \
    using Column = Vector<N, T>;                                                                                                                                         \
    using Row = Vector<M, T>;                                                                                                                                            \
    static constexpr size_type rows = N;                                                                                                                                 \
    static constexpr size_type cols = M;                                                                                                                                 \
    static constexpr size_type size = N * M;
