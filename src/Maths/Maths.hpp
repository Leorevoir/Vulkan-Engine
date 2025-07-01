#pragma once

#include <Engine/Platform.hpp>

#ifndef __VK_ENGINE_MATHS_API
    #define __VK_ENGINE_MATHS_API
#endif

#define VKM_API __VK_ENGINE_MATHS_API

#define VK_MATHS_NOT_NULL(x) (x > 0)
#define VK_MATHS_ASSERT_NUMBERS(N, T) (std::is_arithmetic_v<T>)
#define VK_MATHS_ASSERT_CONVERTIBLE(U, T) (std::convertible_to<U, T>)

#define VK_MATHS_BASE_TEMPLATE(N, T)                                                                                                                                     \
    using value_type = T;                                                                                                                                                \
    using size_type = u32;                                                                                                                                               \
    static constexpr size_type size = N;

#if defined(VKE_ARCH) && (VKE_ARCH != VKE_UNKNOWN)

    #if VKE_ARCH & VKE_ARCH_AVX2
        #include <immintrin.h>
    #elif VKE_ARCH & VKE_ARCH_AVX
        #include <immintrin.h>
    #elif VKE_ARCH & VKE_ARCH_SSE41
        #include <smmintrin.h>
    #elif VKE_ARCH & VKE_ARCH_SSE42
        #if VKE_COMPILER & VKE_CLANG_COMPILER
            #include <popcntintrin.h>
        #endif
        #include <nmmintrin.h>
    #elif VKE_COMPILER & VKE_ARCH_SSE3
        #include <tmmintrin.h>
    #elif VKE_ARCH & VKE_ARCH_SSSE3
        #include <pmmintrin.h>
    #elif VKE_ARCH & VKE_ARCH_SSE2
        #include <emmintrin.h>
    #endif

#endif

#if VKE_ARCH & VKE_ARCH_SSE2
typedef __m128 vke_vec4;
typedef __m128i vke_ivec4;
typedef __m128i vke_uvec4;
#endif

#if VKE_ARCH & VKE_ARCH_AVX
typedef __m256d vke_dvec4;
#endif

#if VKE_ARCH & VKE_ARCH_AVX2
typedef __m256i vke_i64vec4;
typedef __m256i vke_u64vec4;
#endif
