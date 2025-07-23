#pragma once

#include <VKE/Graphics/Context/VulkanContext.hpp>
#include <VKE/Macros.hpp>

#include <memory>

namespace vke {

class VKE_API VulkanObject
{
    public:
        virtual ~VulkanObject() = default;

        virtual void setContext(VulkanContext *context);
        virtual void initialize() = 0;
        virtual void update() = 0;

        template<class T>
        static std::shared_ptr<T> Create(VulkanContext *context)
        {
            assert(context != nullptr && "Context is null");
            std::shared_ptr<T> object = std::make_shared<T>();
            object->setContext(context);
            // object->initialize();
            return object;
        }

    protected:
        VulkanContext *_context = nullptr;
};

}// namespace vke
