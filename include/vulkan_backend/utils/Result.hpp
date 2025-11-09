#pragma once

#include <vulkan/vulkan.h>

#include <exception>
#include <iostream>

#define vk_check(result, message)                                                                                                          \
    do {                                                                                                                                   \
        if ((result) != VK_SUCCESS) {                                                                                                      \
            std::cerr << "Vulkan Error in " << __func__ << " at " << __FILE__ << ":" << __LINE__ << std::endl;                             \
            throw std::runtime_error(message);                                                                                             \
        }                                                                                                                                  \
    } while (0)
