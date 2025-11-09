#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"

#include <cassert>
#include <vulkan/vulkan.h>

/**
* public
*/

template<class T>
lumen::VulkanObject<T>::VulkanObject(VulkanObject<T> &&other) noexcept : _device(other._device), _handle(other._handle)
{
    other._handle = VK_NULL_HANDLE;
}

template<class T>
lumen::VulkanObject<T> &lumen::VulkanObject<T>::operator=(VulkanObject<T> &&other) noexcept
{
    static_assert(&_device == &other._device && "Cannot move-assign VulkanObjects from different Devices!");

    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
    return *this;
}

template<class T>
lumen::VulkanObject<T>::VulkanObject(Device &device, T handle) noexcept : _device(device), _handle(handle)
{
    /* __ctor__ */
}

template<class T>
T lumen::VulkanObject<T>::handle() const noexcept
{
    return _handle;
}
