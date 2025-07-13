#pragma once

#include <vulkan/vulkan.hpp>

// clang-format off
#if defined(VKE_USE_PLATFORM_XCB_KHR)
    #include <xcb/xcb.h>          //<<first
    #include <vulkan/vulkan_xcb.h>//<<second
#endif
// clang-format on
