#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Instance
{
    public:
        Instance(const char *appName);
        ~Instance() noexcept;

        VkInstance handle() const noexcept;

    private:
        void _create_instance(const char *appName);
        void _setup_messenger_debuger();

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
};

}// namespace lumen
