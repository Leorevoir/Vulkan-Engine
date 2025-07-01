#pragma once

#define VKE_UNKNOWN 0x00000000

#define VKE_PLATFORM_WINDOWS 0x00000001
#define VKE_PLATFORM_LINUX 0x00000002
#define VKE_PLATFORM_MACOS 0x00000003

#define VKE_GCC_COMPILER 0x00000001
#define VKE_CLANG_COMPILER 0x00000002
#define VKE_INTEL_COMPILER 0x00000003

#define VKE_ARCH_X86 0x00000001
#define VKE_ARCH_SSE2 0x00000002
#define VKE_ARCH_SSE3 0x00000004
#define VKE_ARCH_SSSE3 0x00000008
#define VKE_ARCH_SSE41 0x00000010
#define VKE_ARCH_SSE42 0x00000020
#define VKE_ARCH_AVX 0x00000040
#define VKE_ARCH_AVX2 0x00000080
#define VKE_ARCH_AVX512 0x00000100
#define VKE_ARCH_ARM 0x00000200
#define VKE_ARCH_NEON 0x00000400
#define VKE_ARCH_MIPS 0x00000800
#define VKE_ARCH_PPC 0x00001000

#if defined(__GNUC__) && !defined(__clang__)
    #define VKE_COMPILER VKE_GCC_COMPILER
#elif defined(__clang__)
    #define VKE_COMPILER VKE_CLANG_COMPILER
#elif defined(__INTEL_COMPILER)
    #define VKE_COMPILER VKE_INTEL_COMPILER
#else
    #define VKE_COMPILER VKE_UNKNOWN
#endif

#if defined(__linux)
    #define VKE_PLATFORM VKE_PLATFORM_LINUX
#elif defined(__APPLE__)
    #define VKE_PLATFORM VKE_PLATFORM_MACOS
#elif defined(_WIN32) || defined(_WIN64)
    #define VKE_PLATFORM VKE_PLATFORM_WINDOWS
#else
    #warning "include/Engine/Platform.hpp: Unknown platform, using VKE_UNKNOWN"
    #define VKE_PLATFORM VKE_UNKNOWN
#endif

#if (VKE_COMPILER == VKE_GCC_COMPILER) || (VKE_COMPILER == VKE_CLANG_COMPILER) || (VKE_COMPILER == VKE_INTEL_COMPILER)
    #define VKE_DEPRECATED __attribute__((deprecated))
    #define VKE_NODISCARD [[nodiscard]]
    #define VKE_NORETURN [[noreturn]]
    #if defined(__GNUC__)
        #define VKE_RESTRICT __restrict__
    #elif defined(_MSC_VER)
        #define VKE_RESTRICT __restrict
    #else
        #define VKE_RESTRICT
    #endif
    #define VKE_VECTOR_CALL __attribute__((vectorcall))
    #define VKE_ALIGNED(x) __attribute__((aligned(x)))
    #define VKE_ALIGNED_STRUCT(type, name, alignment) type name __attribute__((aligned(alignment)))
#endif

#if defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512CD__) && defined(__AVX512VL__) && defined(__AVX512DQ__)
    #define VKE_ARCH VKE_ARCH_AVX512
#elif defined(__AVX2__)
    #define VKE_ARCH VKE_ARCH_AVX2
#elif defined(__AVX__)
    #define VKE_ARCH VKE_ARCH_AVX
#elif defined(__SSE4_2__)
    #define VKE_ARCH VKE_ARCH_SSE42
#elif defined(__SSE4_1__)
    #define VKE_ARCH VKE_ARCH_SSE41
#elif defined(__SSSE3__)
    #define VKE_ARCH VKE_ARCH_SSSE3
#elif defined(__SSE3__)
    #define VKE_ARCH VKE_ARCH_SSE3
#elif defined(__SSE2__)
    #define VKE_ARCH VKE_ARCH_SSE2
#elif defined(__i386__) || defined(__x86_64__)
    #define VKE_ARCH VKE_ARCH_X86
#elif defined(__ARM_NEON)
    #define VKE_ARCH (VKE_ARCH_ARM | VKE_ARCH_NEON)
#elif defined(__arm__)
    #define VKE_ARCH VKE_ARCH_ARM
#elif defined(__mips__)
    #define VKE_ARCH VKE_ARCH_MIPS
#elif defined(__powerpc__)
    #define VKE_ARCH VKE_ARCH_PPC
#else
    #define VKE_ARCH VKE_UNKNOWN
#endif
