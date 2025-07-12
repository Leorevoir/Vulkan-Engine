#pragma once

#include <VK/Window/Platform.hpp>

#include <memory>

namespace vk {

class Window final
{

    public:
        explicit Window(const maths::Vector2u &size, const std::string &title = "VKE Window");
        ~Window();

        void display();
        void event();

    private:
        std::unique_ptr<detail::VK_BackendWindow> _vk_backend;
};

}// namespace vk
