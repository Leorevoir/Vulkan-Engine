#pragma once

#include <VKE/Macros.hpp>

namespace vke {

class VKE_API VulkanEngineInterface
{
    public:
        VulkanEngineInterface() = default;
        virtual ~VulkanEngineInterface() = default;

    protected:
        virtual void initialize() = 0;

        virtual void update() = 0;
        virtual void render() = 0;
};

}// namespace vke
