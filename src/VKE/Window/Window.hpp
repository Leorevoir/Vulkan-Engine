#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Window/Platform/Base.hpp>

#include <memory>

namespace vke {

class VKE_HIDDEN Window final
{

    public:
        explicit Window(const maths::Vector2u &size, const std::string &title = "VKE Window");
        ~Window();

        void createVulkanSurface(VkInstance instance, VkSurfaceKHR &out_surface);

        VKE_NODISCARD bool shouldClose() const;
        VKE_NODISCARD const maths::Vector2u &getSize() const;
        VKE_NODISCARD const std::string &getTitle() const;

        void pollEvents();
        void flush();

    private:
        std::string _title;
        std::unique_ptr<detail::VKE_BackendWindow> _vk_backend;
};

}// namespace vke
