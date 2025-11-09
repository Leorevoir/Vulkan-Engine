#pragma once

namespace lumen {

class Device;

template<class T>
class VulkanObject
{
    public:
        explicit VulkanObject(Device &device, T handle) noexcept;
        virtual ~VulkanObject() = default;

        VulkanObject(const VulkanObject &) = delete;
        VulkanObject &operator=(const VulkanObject &) = delete;

        // Correction : On enlève 'const' car on doit modifier 'other'
        VulkanObject(VulkanObject &&other) noexcept;
        VulkanObject &operator=(VulkanObject &&other) noexcept;

        T handle() const noexcept;

    protected:
        Device &_device;
        T _handle;
};

}// namespace lumen

#include "inline/VulkanObject.inl"
