#pragma once

#include <VK/Window/Platform/Base.hpp>

#include <memory>

namespace vk {

class Window final
{

    public:
        explicit Window(const maths::Vector2u &size, const std::string &title = "VKE Window");
        ~Window();

        VK_NODISCARD bool shouldClose() const;
        VK_NODISCARD const maths::Vector2u &getSize() const;

        void pollEvents();
        void display();

    private:
        std::unique_ptr<detail::VK_BackendWindow> _vk_backend;
};

}// namespace vk
