#pragma once

#if defined(VK_USE_PLATFORM_XCB_KHR)

    #include <VK/Backend.hpp>
    #include <VK/Macros.hpp>
    #include <VK/Window/Platform/Base.hpp>

    #define VK_XCB_EVENT_MASKS                                                                                                                                           \
        XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION                \
            | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE

namespace vk {

namespace detail {

class VK_HIDDEN VK_XCBWindow final : public VK_BackendWindow
{
    public:
        VK_XCBWindow(const maths::Vector2u &size, const std::string &title);
        ~VK_XCBWindow() override;

        void event() override;
        void flush() override;

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

}// namespace vk

#endif
