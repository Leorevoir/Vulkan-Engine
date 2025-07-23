#include <VKE/Error.hpp>
#include <VKE/Window/Window.hpp>

#if defined(VKE_USE_PLATFORM_XCB_KHR)
    #include <VKE/Window/Platform/Xcb.hpp>
#endif

/**
* public
*/

vke::Window::Window(const maths::Vector2u &size, const std::string &title) : _title(title)
{
    if (!VKE_MATHS_NOT_NULL(size.x) && !VKE_MATHS_NOT_NULL(size.y)) {
        throw vke::exception::InvalidArgument("vke::Window::Window", "Window size must be > 0");
    }
#if defined(VKE_USE_PLATFORM_XCB_KHR)
    _vk_backend = std::make_unique<detail::VKE_XCBWindow>(size, title);
#else
    throw vke::exception::NotImplemented("vke::Window::Window", "Platform not supported");
#endif
}

vke::Window::~Window()
{
    /* __dtor__ */
}

void vke::Window::createVulkanSurface(VkInstance instance, VkSurfaceKHR &out_surface) const
{
    _vk_backend->createVulkanSurface(instance, out_surface);
}

bool vke::Window::shouldClose() const
{
    return _vk_backend->_closed == true;
}

const vke::maths::Vector2u &vke::Window::getSize() const
{
    return _vk_backend->_size;
}

const char *vke::Window::getVulkanExtension() const
{
    return _vk_backend->getVulkanExtension();
}

void vke::Window::pollEvents()
{
    _vk_backend->event();
}

void vke::Window::flush()
{
    _vk_backend->flush();
}

const std::string &vke::Window::getTitle() const
{
    return _title;
}
