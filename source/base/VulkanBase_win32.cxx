#include "VulkanBase.h"

#ifdef WIN32

BEGIN_NAMESPACE(VulkanEngine)

VulkanBase* g_vulkanExample;                                                                        \
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)                        \
{                                                                                                    \
if (g_vulkanExample != NULL)                                                                        \
{                                                                                                \
g_vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);                                    \
}                                                                                                \
return (DefWindowProc(hWnd, uMsg, wParam, lParam));                                                \
}

HWND VulkanBase::setupWindow() {
    WNDPROC wndproc = WndProc;
    HINSTANCE hinstance = ::GetModuleHandle(NULL);
    g_vulkanExample = this;
    m_windowInstance = hinstance;

    WNDCLASSEX wndClass;

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = wndproc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hinstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = m_title.c_str();
    wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wndClass))
    {
        std::cout << "Could not register window class!\n";
        fflush(stdout);
        exit(1);
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (m_settings.fullScreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = screenWidth;
        dmScreenSettings.dmPelsHeight = screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if ((m_width != (uint32_t)screenWidth) && (m_height != (uint32_t)screenHeight))
        {
            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                if (MessageBox(NULL, "Fullscreen Mode not supported!\n Switch to window mode?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
                {
                    m_settings.fullScreen = false;
                }
                else
                {
                    m_window = nullptr;
                    return m_window;
                }
            }
        }

    }

    DWORD dwExStyle;
    DWORD dwStyle;

    if (m_settings.fullScreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    }
    else
    {
        if (true) {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        else {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
    }

    RECT windowRect;
    windowRect.left = 0L;
    windowRect.top = 0L;
    windowRect.right = m_settings.fullScreen ? (long)screenWidth : (long)m_width;
    windowRect.bottom = m_settings.fullScreen ? (long)screenHeight : (long)m_height;

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    std::string windowTitle = m_title;
    m_window = CreateWindowEx(0,
        m_title.c_str(),
        windowTitle.c_str(),
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hinstance,
        NULL);

    if (!m_settings.fullScreen)
    {
        // Center on screen
        uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
        uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
        SetWindowPos(m_window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    if (!m_window)
    {
        printf("Could not create window!\n");
        fflush(stdout);
        m_window = nullptr;
        return m_window;
        exit(1);
    }

    ShowWindow(m_window, SW_SHOW);
    SetForegroundWindow(m_window);
    SetFocus(m_window);
    return m_window;
}

void VulkanBase::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		m_prepared = false;
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(m_window, NULL);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		//case KEY_P:
		//	paused = !paused;
		//	break;
		//case KEY_F1:
		//	if (settings.overlay) {
		//		UIOverlay.visible = !UIOverlay.visible;
		//	}
		//	break;
		case KEY_ESCAPE:
			PostQuitMessage(0);
			break;
		}

		//if (camera.firstperson)
		//{
		//	switch (wParam)
		//	{
		//	case KEY_W:
		//		camera.keys.up = true;
		//		break;
		//	case KEY_S:
		//		camera.keys.down = true;
		//		break;
		//	case KEY_A:
		//		camera.keys.left = true;
		//		break;
		//	case KEY_D:
		//		camera.keys.right = true;
		//		break;
		//	}
		//}

		keyPressed((uint32_t)wParam);
		break;
	//case WM_KEYUP:
	//	if (camera.firstperson)
	//	{
	//		switch (wParam)
	//		{
	//		case KEY_W:
	//			camera.keys.up = false;
	//			break;
	//		case KEY_S:
	//			camera.keys.down = false;
	//			break;
	//		case KEY_A:
	//			camera.keys.left = false;
	//			break;
	//		case KEY_D:
	//			camera.keys.right = false;
	//			break;
	//		}
	//	}
	//	break;
	case WM_LBUTTONDOWN:
		m_mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		m_mouseButtons.left = true;
		break;
	case WM_RBUTTONDOWN:
		m_mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		m_mouseButtons.right = true;
		break;
	case WM_MBUTTONDOWN:
		m_mousePos = glm::vec2((float)LOWORD(lParam), (float)HIWORD(lParam));
		m_mouseButtons.middle = true;
		break;
	case WM_LBUTTONUP:
		m_mouseButtons.left = false;
		break;
	case WM_RBUTTONUP:
		m_mouseButtons.right = false;
		break;
	case WM_MBUTTONUP:
		m_mouseButtons.middle = false;
		break;
	case WM_MOUSEWHEEL:
	{
		short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (wheelDelta > 0) {
			m_scroll.up = true;
		}
		else if (wheelDelta < 0) {
			m_scroll.down = true;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		handleMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_SIZE:
		if ((m_prepared) && (wParam != SIZE_MINIMIZED))
		{
			if ((m_resizing) || ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
			{
				m_destWidth = LOWORD(lParam);
				m_destHeight = HIWORD(lParam);
				windowResize();
			}
		}
		break;
	case WM_ENTERSIZEMOVE:
		m_resizing = true;
		break;
	case WM_EXITSIZEMOVE:
		m_resizing = false;
		break;
	}
}

END_NAMESPACE(VulkanEngine)

#endif