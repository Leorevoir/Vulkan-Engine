#pragma once

#include <type_traits>//<< std::is_arithmetic_v

#if defined(VKE_ENGINE_EXPORT_API)
    #ifdef _WIN32
        #define VKEM_API __declspec(dllexport)
    #else
        #define VKEM_API __attribute__((visibility("default")))
    #endif
#else
    #define VKEM_API
#endif

#define VKE_MATHS_NOT_NULL(x) (x > 0)
#define VKE_MATHS_ASSERT_NUMBERS(T) (std::is_arithmetic_v<T>)
#define VKE_MATHS_ASSERT_CONVERTIBLE(U, T) (std::convertible_to<U, T>)

#define VKE_MATHS_ASSERT_VECTOR_SIZE(N) static_assert(N >= 1 && N <= 4, "Vector size must be between 1 and 4");

#define VKE_MATHS_BASE_TEMPLATE(T)                                                                                                                                       \
    using value_type = T;                                                                                                                                                \
    using size_type = u32;

#define VKE_MATHS_VEC_TEMPLATE(N, T)                                                                                                                                     \
    VKE_MATHS_BASE_TEMPLATE(T)                                                                                                                                           \
    static constexpr size_type size = N;

#define VKE_MATHS_MAT_TEMPLATE(N, M, T)                                                                                                                                  \
    VKE_MATHS_BASE_TEMPLATE(T)                                                                                                                                           \
    using Column = Vector<N, T>;                                                                                                                                         \
    using Row = Vector<M, T>;                                                                                                                                            \
    static constexpr size_type size = N;

/** radians = degrees × (π / 180) */
#define VKEM_DEG2RAD(x) ((x) * 0.01745329251994329576923690768489f)

#define VKEM_EPSILON 1e-6f
