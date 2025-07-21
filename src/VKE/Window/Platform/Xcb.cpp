#if defined(VKE_USE_PLATFORM_XCB_KHR)

    #include <VKE/Error.hpp>
    #include <VKE/Memory.hpp>
    #include <VKE/Window/Platform/Xcb.hpp>

/**
 * helpers
 */

static inline xcb_intern_atom_reply_t *_inter_atom_helper(xcb_connection_t *conn, bool only_if_exists, const char *str)
{
    const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, static_cast<u16>(std::strlen(str)), str);

    return xcb_intern_atom_reply(conn, cookie, NULL);
}

/**
* public
*/

vke::detail::VKE_XCBWindow::VKE_XCBWindow(const maths::Vector2u &size, const std::string &title) : VKE_BackendWindow(size, title)
{
    _create();
}

vke::detail::VKE_XCBWindow::~VKE_XCBWindow()
{
    _destroy();
}

void vke::detail::VKE_XCBWindow::event()
{
    if (_closed) {
        return;
    }

    xcb_generic_event_t *event;

    while ((event = xcb_poll_for_event(_connection))) {
        _handle_events(event);
        VKE_SAFE_CLEAN(event, memory::free(event));
    }
}

void vke::detail::VKE_XCBWindow::flush()
{
    if (_closed) {
        return;
    }
    xcb_flush(_connection);
}

const char *vke::detail::VKE_XCBWindow::getVulkanExtension() const
{
    return VK_KHR_XCB_SURFACE_EXTENSION_NAME;
}

void vke::detail::VKE_XCBWindow::createVulkanSurface(VkInstance instance, VkSurfaceKHR &out_surface)
{
    VkXcbSurfaceCreateInfoKHR surface_info = {
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .pNext = VKE_NULLPTR,
        .flags = 0,
        .connection = _connection,
        .window = _window,
    };

    VKE_ASSERT(vkCreateXcbSurfaceKHR(instance, &surface_info, VKE_NULLPTR, &out_surface));
}

/**
 * private
 */

// clang-format off

void vke::detail::VKE_XCBWindow::_create()
{
    _destroy();
    _connect();
    _setup();
    _closed = false;
}

void vke::detail::VKE_XCBWindow::_destroy()
{
    if (_atom_wm_delete_window) {
        VKE_SAFE_CLEAN(_atom_wm_delete_window, memory::free(_atom_wm_delete_window));
        _atom_wm_delete_window = nullptr;
    }
    if (_window) {
        xcb_destroy_window(_connection, _window);
        _window = 0;
    }
    if (_connection) {
        xcb_disconnect(_connection);
        _connection = nullptr;
    }
    _screen = nullptr;
    _closed = true;
}

void vke::detail::VKE_XCBWindow::_connect()
{
    const xcb_setup_t *setup;
    i32 scr;

    _connection = xcb_connect(NULL, &scr);
    if (_connection == NULL) {
        throw vke::exception::RuntimeError("vke::detail::VKE_XCBWindow::_connect", "Failed to connect to X server");
    }

    setup = xcb_get_setup(_connection);

    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    while (scr --> 0) {
        xcb_screen_next(&iter);
    }

    _screen = iter.data;
}

void vke::detail::VKE_XCBWindow::_setup()
{
    const u32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    const u32 value_list[2] = { _screen->black_pixel, VKE_XCB_EVENT_MASKS };

    _window = xcb_generate_id(_connection);

    xcb_create_window(_connection, XCB_COPY_FROM_PARENT, _window, _screen->root, 0, 0, static_cast<u16>(_size.width), static_cast<u16>(_size.height), 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, _screen->root_visual, value_mask, value_list);

    xcb_intern_atom_reply_t *reply_protocols = _inter_atom_helper(_connection, true, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply_delete = _inter_atom_helper(_connection, false, "WM_DELETE_WINDOW");

    xcb_change_property(_connection, XCB_PROP_MODE_REPLACE, _window, reply_protocols->atom, 4, 32, 1, &reply_delete->atom);
    _atom_wm_delete_window = reply_delete;
    VKE_SAFE_CLEAN(reply_protocols, memory::free(reply_protocols));
    xcb_map_window(_connection, _window);
}

void vke::detail::VKE_XCBWindow::_handle_events(xcb_generic_event_t *event)
{
    switch (event->response_type & ~0x80) {

        case XCB_BUTTON_PRESS: {
            const xcb_button_press_event_t *b = reinterpret_cast<xcb_button_press_event_t *>(event);

            switch (b->detail) {
                case XCB_BUTTON_INDEX_4:
                    /* wheel up */
                    break;
                case XCB_BUTTON_INDEX_5:
                    /* wheel down */
                    break;
                default:
                    break;
            }
            break;
        }

        case XCB_BUTTON_RELEASE: {
            /* xcb_button_release_event_t *b = reinterpret_cast<xcb_button_release_event_t *>(event); */
            break;
        }

        case XCB_MOTION_NOTIFY: {
            /* xcb_motion_notify_event_t *m = reinterpret_cast<xcb_motion_notify_event_t *>(event); */
            /* mouse position inside window */
            break;
        }

        case XCB_LEAVE_NOTIFY: {
            /* xcb_leave_notify_event_t *l = reinterpret_cast<xcb_leave_notify_event_t *>(event); */
            /* mouse leave window */
            break;
        }

        case XCB_KEY_PRESS: {
            /* xcb_key_press_event_t *k = reinterpret_cast<xcb_key_press_event_t *>(event); */
            /* key press */
            break;
        }

        case XCB_KEY_RELEASE: {
            /* xcb_key_release_event_t *k = reinterpret_cast<xcb_key_release_event_t *>(event); */
            /* key release */
            break;
        }

        case XCB_CLIENT_MESSAGE:
            if ((*(xcb_client_message_event_t *) event).data.data32[0] == (*_atom_wm_delete_window).atom) {
                _closed = true;
            }
            break;

        case XCB_DESTROY_NOTIFY:
            _closed = true;
            break;

        default:
            break;
    }
}

// clang-format on

#endif
