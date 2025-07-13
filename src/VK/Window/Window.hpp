#pragma once

#include <VK/Macros.hpp>
#include <VK/Window/Platform/Base.hpp>

#include <memory>

namespace vk {

class VK_HIDDEN Window final
{

    public:
        explicit Window(const maths::Vector2u &size, const std::string &title = "VKE Window");
        ~Window();

        VK_NODISCARD bool shouldClose() const;
        VK_NODISCARD const maths::Vector2u &getSize() const;

        void pollEvents();
        void flush();

    private:
        std::unique_ptr<detail::VK_BackendWindow> _vk_backend;
};

}// namespace vk
