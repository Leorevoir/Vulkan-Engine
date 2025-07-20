#pragma once

#include <VKE/Error.hpp>
#include <VKE/Types.hpp>

#include <cstring>

namespace vke {

namespace memory {

static inline void copy(void *dst, const void *src, usize size)
{
    std::memcpy(dst, src, size);
}

template<typename Object>
static inline Object *allocate(usize size)
{
    Object *ptr = new Object[size];

    if (!ptr) {
        throw exception::BadAlloc("vk::memory::allocate", "Failed to allocate memory file: ", __FILE__, " line: ", __LINE__);
    }
    return ptr;
}

template<typename Object, typename... Args>
static inline Object *allocate(Args &&...args)
{
    Object *ptr = new Object(std::forward<Args>(args)...);

    if (!ptr) {
        throw exception::BadAlloc("vk::memory::allocate", "Failed to allocate memory file: ", __FILE__, " line: ", __LINE__);
    }
    return ptr;
}

template<typename Object>
static inline void clean(Object *&ptr)
{
    if (ptr) {
        delete ptr;
        ptr = nullptr;
    }
}

template<typename ArrayObjects>
static inline void clean_array(ArrayObjects *&ptr)
{
    if (ptr) {
        delete[] ptr;
        ptr = nullptr;
    }
}

template<typename C_Object>
static inline void free(C_Object *&ptr)
{
    if (ptr) {
        std::free(ptr);
        ptr = nullptr;
    }
}

}// namespace memory

}// namespace vke
