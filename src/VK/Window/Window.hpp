#pragma once

#include <VK/Window/Platform/Base.hpp>

#if defined(__linux__)
    #include <VK/Window/Platform/X11.hpp>
#endif

#include <memory>

namespace vk {

class Window final
{

    public:
        explicit Window(const maths::Vector2u &size, const std::string &title = "VKE Window");
        ~Window();

        VK_NODISCARD bool shouldClose() const;
        void display();
        void event();

    private:
        std::unique_ptr<detail::VK_BackendWindow> _vk_backend;
};

}// namespace vk
