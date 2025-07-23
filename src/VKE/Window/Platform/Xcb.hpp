#pragma once

#if defined(VKE_USE_PLATFORM_XCB_KHR)

    #include <VKE/Macros.hpp>
    #include <VKE/Window/Platform/Base.hpp>

    #define VKE_XCB_EVENT_MASKS                                                                                                                                          \
        XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION                \
            | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE

namespace vke {

namespace detail {

class VKE_API VKE_XCBWindow final : public VKE_BackendWindow
{
    public:
        VKE_XCBWindow(const maths::Vector2u &size, const std::string &title);
        ~VKE_XCBWindow() override;

        void event() override;
        void flush() override;
        const char *getVulkanExtension() const override;
        void createVulkanSurface(VkInstance instance, VkSurfaceKHR &out_surface) const override;

    private:
        void _create();
        void _connect();
        void _setup();

        void _handle_events(xcb_generic_event_t *event);

        void _destroy();

        xcb_connection_t *_connection = nullptr;
        xcb_screen_t *_screen = nullptr;
        xcb_window_t _window = 0;
        xcb_intern_atom_reply_t *_atom_wm_delete_window = nullptr;
};

}// namespace detail

}// namespace vke

#endif
