#pragma once

#if defined(VK_ENGINE_EXPORT_API)
    #ifdef _WIN32
        #define VK_API __declspec(dllexport)
    #else
        #define VK_API __attribute__((visibility("default")))
    #endif
#else
    #define VK_API
#endif

#define VK_HIDDEN __attribute__((visibility("hidden")))

/**
 * NOTE: if you want to force NO inline, you can define VK_FORCE_NO_INLINE
 */

#if defined(VK_FORCE_NO_INLINE)
    #define VK_INLINE
#else
    #define VK_INLINE inline
#endif

/**
* NOTE: we compile in -std=gnu++20 so no need to check for C++20 support
*/

#ifndef VK_NODISCARD
    #define VK_NODISCARD [[nodiscard]]
#endif

#ifndef VK_UNUSED
    #define VK_UNUSED __attribute__((unused))
#endif

#ifndef VK_CLEANUP
    #define VK_CLEANUP(func) __attribute__((cleanup(func)))
#endif

#define VK_NULL_PTR nullptr
#define VK_CONSTEXPR constexpr

#define __VK_RV(r)                                                                                                                                                       \
    case VK_##r:                                                                                                                                                         \
        return #r

#define VK_GET_ERROR_STRING(result)                                                                                                                                      \
    ([&]() -> const char * {                                                                                                                                             \
        switch (result) {                                                                                                                                                \
            __VK_RV(NOT_READY);                                                                                                                                          \
            __VK_RV(TIMEOUT);                                                                                                                                            \
            __VK_RV(EVENT_SET);                                                                                                                                          \
            __VK_RV(EVENT_RESET);                                                                                                                                        \
            __VK_RV(INCOMPLETE);                                                                                                                                         \
            __VK_RV(ERROR_OUT_OF_HOST_MEMORY);                                                                                                                           \
            __VK_RV(ERROR_OUT_OF_DEVICE_MEMORY);                                                                                                                         \
            __VK_RV(ERROR_INITIALIZATION_FAILED);                                                                                                                        \
            __VK_RV(ERROR_DEVICE_LOST);                                                                                                                                  \
            __VK_RV(ERROR_MEMORY_MAP_FAILED);                                                                                                                            \
            __VK_RV(ERROR_LAYER_NOT_PRESENT);                                                                                                                            \
            __VK_RV(ERROR_EXTENSION_NOT_PRESENT);                                                                                                                        \
            __VK_RV(ERROR_FEATURE_NOT_PRESENT);                                                                                                                          \
            __VK_RV(ERROR_INCOMPATIBLE_DRIVER);                                                                                                                          \
            __VK_RV(ERROR_TOO_MANY_OBJECTS);                                                                                                                             \
            __VK_RV(ERROR_FORMAT_NOT_SUPPORTED);                                                                                                                         \
            __VK_RV(ERROR_SURFACE_LOST_KHR);                                                                                                                             \
            __VK_RV(ERROR_NATIVE_WINDOW_IN_USE_KHR);                                                                                                                     \
            __VK_RV(SUBOPTIMAL_KHR);                                                                                                                                     \
            __VK_RV(ERROR_OUT_OF_DATE_KHR);                                                                                                                              \
            __VK_RV(ERROR_INCOMPATIBLE_DISPLAY_KHR);                                                                                                                     \
            __VK_RV(ERROR_VALIDATION_FAILED_EXT);                                                                                                                        \
            __VK_RV(ERROR_INVALID_SHADER_NV);                                                                                                                            \
            default:                                                                                                                                                     \
                return "UNKNOWN_ERROR";                                                                                                                                  \
        }                                                                                                                                                                \
    })()

#define VK_SAFE_CLEAN(n, func)                                                                                                                                           \
    do {                                                                                                                                                                 \
        if ((n) != VK_NULL_PTR) {                                                                                                                                        \
            func(n);                                                                                                                                                     \
            (n) = VK_NULL_PTR;                                                                                                                                           \
        }                                                                                                                                                                \
    } while (0)

#define VK_ASSERT(expr)                                                                                                                                                  \
    do {                                                                                                                                                                 \
        const VkResult result = (expr);                                                                                                                                  \
        if (result != VK_SUCCESS) {                                                                                                                                      \
            throw vk::exception::RuntimeError("Vulkan fatal error", VK_GET_ERROR_STRING(result), " file: ", __FILE__, ", line: ", __LINE__);                             \
        }                                                                                                                                                                \
    } while (0)
