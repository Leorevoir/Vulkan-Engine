#pragma once

#include <cstdlib>

namespace vk {

namespace utils {

static inline void clean(void *ptr)
{
    if (!ptr) {
        return;
    }
    ::free(ptr);
    ptr = nullptr;
}

}// namespace utils

}// namespace vk
