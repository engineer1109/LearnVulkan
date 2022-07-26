//
// Created by wjl on 2021/4/7.
//

#include "VulkanBase.h"

BEGIN_NAMESPACE(VulkanEngine)

#if defined(VK_USE_PLATFORM_XCB_KHR)

static inline xcb_intern_atom_reply_t* intern_atom_helper(xcb_connection_t *conn, bool only_if_exists, const char *str)
{
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, strlen(str), str);
    return xcb_intern_atom_reply(conn, cookie, NULL);
}

void VulkanBase::initxcbConnection()
{
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    m_connection = xcb_connect(NULL, &scr);
    if (m_connection == NULL) {
        printf("Could not find a compatible Vulkan ICD!\n");
        fflush(stdout);
        exit(1);
    }

    setup = xcb_get_setup(m_connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0)
        xcb_screen_next(&iter);
    m_screen = iter.data;
}

xcb_window_t VulkanBase::setupWindow() {
    uint32_t value_mask, value_list[32];

    m_window = xcb_generate_id(m_connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = m_screen->black_pixel;
    value_list[1] =
            XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_KEY_PRESS |
            XCB_EVENT_MASK_EXPOSURE |
            XCB_EVENT_MASK_STRUCTURE_NOTIFY |
            XCB_EVENT_MASK_POINTER_MOTION |
            XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE;

    if (m_settings.fullScreen) {
        m_width = m_destWidth = m_screen->width_in_pixels;
        m_height = m_destHeight = m_screen->height_in_pixels;
    }

    xcb_create_window(m_connection,
                      XCB_COPY_FROM_PARENT,
                      m_window, m_screen->root,
                      0, 0, m_width, m_height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      m_screen->root_visual,
                      value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_reply_t *reply = intern_atom_helper(m_connection, true, "WM_PROTOCOLS");
    m_atom_wm_delete_window = intern_atom_helper(m_connection, false, "WM_DELETE_WINDOW");

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE,
                        m_window, (*reply).atom, 4, 32, 1,
                        &(*m_atom_wm_delete_window).atom);

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE,
                        m_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                        m_title.size(), m_title.c_str());

    free(reply);

    if (m_settings.fullScreen) {
        xcb_intern_atom_reply_t *atom_wm_state = intern_atom_helper(m_connection, false, "_NET_WM_STATE");
        xcb_intern_atom_reply_t *atom_wm_fullscreen = intern_atom_helper(m_connection, false,
                                                                         "_NET_WM_STATE_FULLSCREEN");
        xcb_change_property(m_connection,
                            XCB_PROP_MODE_REPLACE,
                            m_window, atom_wm_state->atom,
                            XCB_ATOM_ATOM, 32, 1,
                            &(atom_wm_fullscreen->atom));
        free(atom_wm_fullscreen);
        free(atom_wm_state);
    }

    xcb_map_window(m_connection, m_window);

    return (m_window);
}

void VulkanBase::handleEvent(const xcb_generic_event_t *event)
{
    switch (event->response_type & 0x7f)
    {
        case XCB_CLIENT_MESSAGE:
            if ((*(xcb_client_message_event_t*)event).data.data32[0] ==
                (*m_atom_wm_delete_window).atom) {
                m_quit = true;
            }
            break;
        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
            handleMouseMove((int32_t)motion->event_x, (int32_t)motion->event_y);
            break;
        }
            break;
        case XCB_BUTTON_PRESS:
        {
            xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
            if (press->detail == XCB_BUTTON_INDEX_1)
                m_mouseButtons.left = true;
            if (press->detail == XCB_BUTTON_INDEX_2)
                m_mouseButtons.middle = true;
            if (press->detail == XCB_BUTTON_INDEX_3)
                m_mouseButtons.right = true;
            if (press->detail == XCB_BUTTON_INDEX_4) {
                LOG("scroll up\n");
                m_scroll.up = true;
            }
            if (press->detail == XCB_BUTTON_INDEX_5){
                LOG("scroll down\n");
                m_scroll.down = true;
            }
        }
            break;
        case XCB_BUTTON_RELEASE:
        {
            xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
            if (press->detail == XCB_BUTTON_INDEX_1)
                m_mouseButtons.left = false;
            if (press->detail == XCB_BUTTON_INDEX_2)
                m_mouseButtons.middle = false;
            if (press->detail == XCB_BUTTON_INDEX_3)
                m_mouseButtons.right = false;
//            if (press->detail == XCB_BUTTON_INDEX_4)
//                m_scroll.up = false;
//            if (press->detail == XCB_BUTTON_INDEX_5)
//                m_scroll.down = false;
        }
            break;
//        case XCB_KEY_PRESS:
//        {
//            const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
//            switch (keyEvent->detail)
//            {
//                case KEY_W:
//                    camera.keys.up = true;
//                    break;
//                case KEY_S:
//                    camera.keys.down = true;
//                    break;
//                case KEY_A:
//                    camera.keys.left = true;
//                    break;
//                case KEY_D:
//                    camera.keys.right = true;
//                    break;
//                case KEY_P:
//                    paused = !paused;
//                    break;
//                case KEY_F1:
//                    if (settings.overlay) {
//                        settings.overlay = !settings.overlay;
//                    }
//                    break;
//            }
//        }
//            break;
//        case XCB_KEY_RELEASE:
//        {
//            const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
//            switch (keyEvent->detail)
//            {
//                case KEY_W:
//                    camera.keys.up = false;
//                    break;
//                case KEY_S:
//                    camera.keys.down = false;
//                    break;
//                case KEY_A:
//                    camera.keys.left = false;
//                    break;
//                case KEY_D:
//                    camera.keys.right = false;
//                    break;
//                case KEY_ESCAPE:
//                    quit = true;
//                    break;
//            }
//            keyPressed(keyEvent->detail);
//        }
            break;
        case XCB_DESTROY_NOTIFY:
            m_quit = true;
            break;
        case XCB_CONFIGURE_NOTIFY:
        {
            const xcb_configure_notify_event_t *cfgEvent = (const xcb_configure_notify_event_t *)event;
            if ((m_prepared) && ((cfgEvent->width != m_width) || (cfgEvent->height != m_height)))
            {
                m_destWidth = cfgEvent->width;
                m_destHeight = cfgEvent->height;
                if ((m_destWidth > 0) && (m_destHeight > 0))
                {
                    windowResize();
                }
            }
        }
            break;
        default:
            break;
    }
}

#endif

END_NAMESPACE(VulkanEngine)