#pragma once

#if defined(VKE_ENGINE_EXPORT_API)
    #ifdef _WIN32
        #define VKE_API __declspec(dllexport)
    #else
        #define VKE_API __attribute__((visibility("default")))
    #endif
#else
    #define VKE_API
#endif

#define VKE_HIDDEN __attribute__((visibility("hidden")))

/**
 * NOTE: if you want to force NO inline, you can define VKE_FORCE_NO_INLINE
 */

#if defined(VKE_FORCE_NO_INLINE)
    #define VKE_INLINE
#else
    #define VKE_INLINE inline
#endif

/**
* NOTE: we compile in -std=gnu++20 so no need to check for C++20 support
*/

#ifndef VKE_NODISCARD
    #define VKE_NODISCARD [[nodiscard]]
#endif

#ifndef VKE_UNUSED
    #define VKE_UNUSED __attribute__((unused))
#endif

#ifndef VKE_CLEANUP
    #define VKE_CLEANUP(func) __attribute__((cleanup(func)))
#endif

#define VKE_NULL_PTR nullptr
#define VKE_CONSTEXPR constexpr

#define VKE_SUCCESS VK_SUCCESS
#define VKE_TRUE VK_TRUE
#define VKE_FALSE VK_FALSE

#define __VKE_RV(r)                                                                                                                                                      \
    case VK_##r:                                                                                                                                                         \
        return #r

#define VKE_GET_ERROR_STRING(result)                                                                                                                                     \
    ([&]() -> const char * {                                                                                                                                             \
        switch (result) {                                                                                                                                                \
            __VKE_RV(NOT_READY);                                                                                                                                         \
            __VKE_RV(TIMEOUT);                                                                                                                                           \
            __VKE_RV(EVENT_SET);                                                                                                                                         \
            __VKE_RV(EVENT_RESET);                                                                                                                                       \
            __VKE_RV(INCOMPLETE);                                                                                                                                        \
            __VKE_RV(ERROR_OUT_OF_HOST_MEMORY);                                                                                                                          \
            __VKE_RV(ERROR_OUT_OF_DEVICE_MEMORY);                                                                                                                        \
            __VKE_RV(ERROR_INITIALIZATION_FAILED);                                                                                                                       \
            __VKE_RV(ERROR_DEVICE_LOST);                                                                                                                                 \
            __VKE_RV(ERROR_MEMORY_MAP_FAILED);                                                                                                                           \
            __VKE_RV(ERROR_LAYER_NOT_PRESENT);                                                                                                                           \
            __VKE_RV(ERROR_EXTENSION_NOT_PRESENT);                                                                                                                       \
            __VKE_RV(ERROR_FEATURE_NOT_PRESENT);                                                                                                                         \
            __VKE_RV(ERROR_INCOMPATIBLE_DRIVER);                                                                                                                         \
            __VKE_RV(ERROR_TOO_MANY_OBJECTS);                                                                                                                            \
            __VKE_RV(ERROR_FORMAT_NOT_SUPPORTED);                                                                                                                        \
            __VKE_RV(ERROR_SURFACE_LOST_KHR);                                                                                                                            \
            __VKE_RV(ERROR_NATIVE_WINDOW_IN_USE_KHR);                                                                                                                    \
            __VKE_RV(SUBOPTIMAL_KHR);                                                                                                                                    \
            __VKE_RV(ERROR_OUT_OF_DATE_KHR);                                                                                                                             \
            __VKE_RV(ERROR_INCOMPATIBLE_DISPLAY_KHR);                                                                                                                    \
            __VKE_RV(ERROR_VALIDATION_FAILED_EXT);                                                                                                                       \
            __VKE_RV(ERROR_INVALID_SHADER_NV);                                                                                                                           \
            default:                                                                                                                                                     \
                return "UNKNOWN_ERROR";                                                                                                                                  \
        }                                                                                                                                                                \
    })()

#define VKE_SAFE_CLEAN(n, func)                                                                                                                                          \
    do {                                                                                                                                                                 \
        if ((n) != VKE_NULL_PTR) {                                                                                                                                       \
            (func);                                                                                                                                                      \
            (n) = VKE_NULL_PTR;                                                                                                                                          \
        }                                                                                                                                                                \
    } while (0)

#define VKE_ASSERT(expr)                                                                                                                                                 \
    do {                                                                                                                                                                 \
        const VkResult result = (expr);                                                                                                                                  \
        if (result != VKE_SUCCESS) {                                                                                                                                     \
            throw vke::exception::RuntimeError("Vulkan fatal error", VKE_GET_ERROR_STRING(result), " file: ", __FILE__, ", line: ", __LINE__);                           \
        }                                                                                                                                                                \
    } while (0)
