#pragma once

#include <vulkan_backend/Backend.hpp>

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
