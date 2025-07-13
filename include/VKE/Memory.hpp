#pragma once

#include <VKE/Error.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace memory {

template<typename Object>
static inline Object *allocate(usize size)
{
    Object *ptr = new Object[size];

    if (!ptr) {
        throw exception::BadAlloc("vk::memory::allocate", "Failed to allocate memory");
    }
    return ptr;
}

template<typename Object, typename... Args>
static inline Object *allocate(Args &&...args)
{
    Object *ptr = new Object(std::forward<Args>(args)...);

    if (!ptr) {
        throw exception::BadAlloc("vk::memory::allocate", "Failed to allocate memory");
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
