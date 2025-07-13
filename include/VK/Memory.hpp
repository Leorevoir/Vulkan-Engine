#pragma once

#include <VK/Error.hpp>
#include <VK/Types.hpp>

#include <cstdlib>
#include <cstring>

namespace vk {

namespace memory {

using Object = void;

static inline void clear(Object *ptr, usize size)
{
    if (ptr) {
        std::memset(ptr, 0, size);
    }
}

static inline void *allocate(usize size)
{
    Object *ptr = std::malloc(size);

    if (!ptr) {
        throw exception::BadAlloc("vk::memory::allocate", "Failed to allocate memory");
    }
    return ptr;
}

static inline void clean(Object *ptr)
{
    std::free(ptr);
}

}// namespace memory

}// namespace vk
