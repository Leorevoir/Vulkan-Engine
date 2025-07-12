#if defined(VK_USE_PLATFORM_XCB_KHR)

    #include <VK/Error.hpp>
    #include <VK/Utils.hpp>
    #include <VK/Window/Platform/Xcb.hpp>

/**
* public
*/

vk::detail::VK_XCBWindow::VK_XCBWindow(const maths::Vector2u &size, const std::string &title) : VK_BackendWindow(size, title)
{
    _create();
}

vk::detail::VK_XCBWindow::~VK_XCBWindow()
{
    _destroy();
}

void vk::detail::VK_XCBWindow::event()
{
    if (_closed) {
        return;
    }

    xcb_generic_event_t *event;

    while ((event = xcb_poll_for_event(_connection))) {
        _handle_events(event);
        utils::clean(event);
    }
}

void vk::detail::VK_XCBWindow::display()
{
    if (_closed) {
        return;
    }
    xcb_flush(_connection);
}

/**
 * helpers
 */

    #include <cstring>

static inline xcb_intern_atom_reply_t *_inter_atom_helper(xcb_connection_t *conn, bool only_if_exists, const char *str)
{
    const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, static_cast<u16>(std::strlen(str)), str);

    return xcb_intern_atom_reply(conn, cookie, NULL);
}

/**
 * private
 */

// clang-format off

void vk::detail::VK_XCBWindow::_create()
{
    _destroy();
    _connect();
    _setup();
    _closed = false;
}

void vk::detail::VK_XCBWindow::_destroy()
{
    if (_closed) {
        return;
    }
    if (_atom_wm_delete_window) {
        utils::clean(_atom_wm_delete_window);
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

void vk::detail::VK_XCBWindow::_connect()
{
    const xcb_setup_t *setup;
    i32 scr;

    _connection = xcb_connect(NULL, &scr);
    if (_connection == NULL) {
        throw vk::exception::RuntimeError("vk::detail::VK_XCBWindow::_connect", "Failed to connect to X server");
    }

    setup = xcb_get_setup(_connection);

    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    while (scr --> 0) {
        xcb_screen_next(&iter);
    }

    _screen = iter.data;
}

void vk::detail::VK_XCBWindow::_setup()
{
    const u32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    const u32 value_list[2] = { _screen->black_pixel, VK_XCB_EVENT_MASKS };

    _window = xcb_generate_id(_connection);

    xcb_create_window(_connection, XCB_COPY_FROM_PARENT, _window, _screen->root, 0, 0, static_cast<u16>(_size.width), static_cast<u16>(_size.height), 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, _screen->root_visual, value_mask, value_list);

    xcb_intern_atom_reply_t *reply_protocols = _inter_atom_helper(_connection, true, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply_delete = _inter_atom_helper(_connection, false, "WM_DELETE_WINDOW");

    if (reply_protocols && reply_delete) {
        xcb_change_property(_connection, XCB_PROP_MODE_REPLACE, _window, reply_protocols->atom, 4, 32, 1, &reply_delete->atom);
        _atom_wm_delete_window = reply_delete;
    } else {
        utils::clean(reply_delete);
    }

    utils::clean(reply_protocols);
    xcb_map_window(_connection, _window);
}

void vk::detail::VK_XCBWindow::_handle_events(xcb_generic_event_t *event)
{
    switch (event->response_type & 0x7f) {

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
