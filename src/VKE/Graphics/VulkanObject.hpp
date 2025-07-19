#pragma once

#include <VKE/Graphics/Context/VulkanContext.hpp>
#include <VKE/Macros.hpp>

namespace vke {

class VKE_API VulkanObject
{

    public:
        VulkanObject(VulkanContext *context) : _context(context)
        {
        }

        virtual ~VulkanObject() = default;

        virtual void update() = 0;

    protected:
        VulkanContext *_context = nullptr;
};

}// namespace vke
