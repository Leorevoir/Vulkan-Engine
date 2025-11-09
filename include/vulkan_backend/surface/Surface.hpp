#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace lumen {

class Instance;

class Surface
{
    public:
        Surface(Instance &instance, GLFWwindow *window);
        ~Surface() noexcept;

        VkSurfaceKHR handle() const noexcept;

    private:
        VkSurfaceKHR _surface;
        Instance &_instance;
};

}// namespace lumen
