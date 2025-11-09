#pragma once

#include <vulkan_backend/Backend.hpp>

#include <cstdint>
#include <vector>

namespace lumen::config {

static constexpr uint32_t WINDOW_WIDTH = 800;
static constexpr uint32_t WINDOW_HEIGHT = 600;
static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

static const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
static const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#if defined(LUMEN_DEBUG)
static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#else
static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#endif

}// namespace lumen::config
