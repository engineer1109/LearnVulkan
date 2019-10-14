#include "vulkan_basicengine.h"
VulkanBasicEngine::VulkanBasicEngine(bool enableValidation):VulkanExampleBase(enableValidation){

}

VulkanBasicEngine::~VulkanBasicEngine(){

}

std::string VulkanBasicEngine::getWindowTitle()
{
	std::string device(deviceProperties.deviceName);
	std::string windowTitle;
	windowTitle = title + " - " + device;
	if (!settings.overlay) {
		windowTitle += " - " + std::to_string(frameCounter) + " fps";
	}
	return windowTitle;
}

#ifdef __unix__
void VulkanBasicEngine::initWindow(){
    this->setupWindow();
}

void VulkanBasicEngine::setWindow(uint32_t window){
    this->window=window;
    xcb_map_window(connection,window);
}
#endif

#ifdef WIN32

void VulkanBasicEngine::setWindow(uint32_t window){
    this->window=(HWND)window;
    this->windowInstance = ::GetModuleHandle(NULL);
}

VulkanBasicEngine *g_vulkanExample;                                                                        \
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)                        \
{                                                                                                    \
    if (g_vulkanExample != NULL)                                                                        \
    {                                                                                                \
        g_vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);                                    \
    }                                                                                                \
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));                                                \
}

void VulkanBasicEngine::initWindow()
{
    WNDPROC wndproc=WndProc;
    HINSTANCE hinstance = ::GetModuleHandle(NULL);
    g_vulkanExample=this;
    this->windowInstance = hinstance;

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
    wndClass.lpszClassName = name.c_str();
    wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wndClass))
    {
        std::cout << "Could not register window class!\n";
        fflush(stdout);
        exit(1);
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (settings.fullscreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = screenWidth;
        dmScreenSettings.dmPelsHeight = screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if ((width != (uint32_t)screenWidth) && (height != (uint32_t)screenHeight))
        {
            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                if (MessageBox(NULL, "Fullscreen Mode not supported!\n Switch to window mode?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
                {
                    settings.fullscreen = false;
                }
                else
                {
                    window=nullptr;
                    return;
                }
            }
        }

    }

    DWORD dwExStyle;
    DWORD dwStyle;

    if (settings.fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    }
    else
    {
        if(m_enableWindow){
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
            dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        else{
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
    }

    RECT windowRect;
    windowRect.left = 0L;
    windowRect.top = 0L;
    windowRect.right = settings.fullscreen ? (long)screenWidth : (long)width;
    windowRect.bottom = settings.fullscreen ? (long)screenHeight : (long)height;

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    std::string windowTitle = getWindowTitle();
    window = CreateWindowEx(0,
        name.c_str(),
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

    if (!settings.fullscreen)
    {
        // Center on screen
        uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
        uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
        SetWindowPos(window, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    if (!window)
    {
        printf("Could not create window!\n");
        fflush(stdout);
        window=nullptr;
        return;
        exit(1);
    }

    ShowWindow(window, SW_SHOW);
    SetForegroundWindow(window);
    SetFocus(window);
}
#endif

void VulkanBasicEngine::renderLoop()
{
    if (benchmark.active) {
        benchmark.run([=] { render(); }, vulkanDevice->properties);
        vkDeviceWaitIdle(device);
        if (benchmark.filename != "") {
            benchmark.saveResults();
        }
        return;
    }

    destWidth = width;
    destHeight = height;
    lastTimestamp = std::chrono::high_resolution_clock::now();
#if defined(_WIN32)
    MSG msg;
    bool quitMessageReceived = false;
    while (!quitMessageReceived and !m_quit) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                quitMessageReceived = true;
                break;
            }
        }
        if (!IsIconic(window)) {
            renderFrame();
        }
    }
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    while (!m_quit)
    {
        int ident;
        int events;
        struct android_poll_source* source;
        bool destroy = false;

        focused = true;

        while ((ident = ALooper_pollAll(focused ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
        {
            if (source != NULL)
            {
                source->process(androidApp, source);
            }
            if (androidApp->destroyRequested != 0)
            {
                LOGD("Android app destroy requested");
                destroy = true;
                break;
            }
        }

        // App destruction requested
        // Exit loop, example will be destroyed in application main
        if (destroy)
        {
            ANativeActivity_finish(androidApp->activity);
            break;
        }

        // Render frame
        if (prepared)
        {
            auto tStart = std::chrono::high_resolution_clock::now();
            render();
            frameCounter++;
            auto tEnd = std::chrono::high_resolution_clock::now();
            auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
            frameTimer = tDiff / 1000.0f;
            camera.update(frameTimer);
            // Convert to clamped timer value
            if (!paused)
            {
                timer += timerSpeed * frameTimer;
                if (timer > 1.0)
                {
                    timer -= 1.0f;
                }
            }
            float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
            if (fpsTimer > 1000.0f)
            {
                lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
                frameCounter = 0;
                lastTimestamp = tEnd;
            }

            // TODO: Cap UI overlay update rates/only issue when update requested
            updateOverlay();

            bool updateView = false;

            // Check touch state (for movement)
            if (touchDown) {
                touchTimer += frameTimer;
            }
            if (touchTimer >= 1.0) {
                camera.keys.up = true;
                viewChanged();
            }

            // Check gamepad state
            const float deadZone = 0.0015f;
            // todo : check if gamepad is present
            // todo : time based and relative axis positions
            if (camera.type != Camera::CameraType::firstperson)
            {
                // Rotate
                if (std::abs(gamePadState.axisLeft.x) > deadZone)
                {
                    rotation.y += gamePadState.axisLeft.x * 0.5f * rotationSpeed;
                    camera.rotate(glm::vec3(0.0f, gamePadState.axisLeft.x * 0.5f, 0.0f));
                    updateView = true;
                }
                if (std::abs(gamePadState.axisLeft.y) > deadZone)
                {
                    rotation.x -= gamePadState.axisLeft.y * 0.5f * rotationSpeed;
                    camera.rotate(glm::vec3(gamePadState.axisLeft.y * 0.5f, 0.0f, 0.0f));
                    updateView = true;
                }
                // Zoom
                if (std::abs(gamePadState.axisRight.y) > deadZone)
                {
                    zoom -= gamePadState.axisRight.y * 0.01f * zoomSpeed;
                    updateView = true;
                }
                if (updateView)
                {
                    viewChanged();
                }
            }
            else
            {
                updateView = camera.updatePad(gamePadState.axisLeft, gamePadState.axisRight, frameTimer);
                if (updateView)
                {
                    viewChanged();
                }
            }
        }
    }
#elif defined(_DIRECT2DISPLAY)
    while (!quit and !m_quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();
        if (viewUpdated)
        {
            viewUpdated = false;
            viewChanged();
        }
        render();
        frameCounter++;
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = tDiff / 1000.0f;
        camera.update(frameTimer);
        if (camera.moving())
        {
            viewUpdated = true;
        }
        // Convert to clamped timer value
        if (!paused)
        {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
                timer -= 1.0f;
            }
        }
        float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
        if (fpsTimer > 1000.0f)
        {
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
        }
        updateOverlay();
    }
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    while (!quit and !m_quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();
        if (viewUpdated)
        {
            viewUpdated = false;
            viewChanged();
        }

        while (!configured)
            wl_display_dispatch(display);
        while (wl_display_prepare_read(display) != 0)
            wl_display_dispatch_pending(display);
        wl_display_flush(display);
        wl_display_read_events(display);
        wl_display_dispatch_pending(display);

        render();
        frameCounter++;
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = tDiff / 1000.0f;
        camera.update(frameTimer);
        if (camera.moving())
        {
            viewUpdated = true;
        }
        // Convert to clamped timer value
        if (!paused)
        {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
                timer -= 1.0f;
            }
        }
        float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
        if (fpsTimer > 1000.0f)
        {
            if (!settings.overlay)
            {
                std::string windowTitle = getWindowTitle();
                xdg_toplevel_set_title(xdg_toplevel, windowTitle.c_str());
            }
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
        }
        updateOverlay();
    }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    xcb_flush(connection);
    while (!quit and !m_quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();
        if (viewUpdated)
        {
            viewUpdated = false;
            viewChanged();
        }
        xcb_generic_event_t *event;
        while ((event = xcb_poll_for_event(connection)))
        {
            handleEvent(event);
            free(event);
        }
        render();
        frameCounter++;
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = tDiff / 1000.0f;
        camera.update(frameTimer);
        if (camera.moving())
        {
            viewUpdated = true;
        }
        // Convert to clamped timer value
        if (!paused)
        {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
                timer -= 1.0f;
            }
        }
        float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
        if (fpsTimer > 1000.0f)
        {
            if (!settings.overlay)
            {
                std::string windowTitle = getWindowTitle();
                xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
                    window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                    windowTitle.size(), windowTitle.c_str());
            }
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
        }
        updateOverlay();
    }
#endif
    // Flush device to make sure all resources can be freed
    if (device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device);
    }
}

std::string VulkanBasicEngine::getShaderPath(){
    return "../data/";
}

void VulkanBasicEngine::quitRender(){
    this->m_quit=true;
}

void VulkanBasicEngine::preRender(){
    destWidth = width;
    destHeight = height;
    lastTimestamp = std::chrono::high_resolution_clock::now();
}

void VulkanBasicEngine::singleRender(){
    //while (!quit and !m_quit)
    //{
        auto tStart = std::chrono::high_resolution_clock::now();
        if (viewUpdated)
        {
            viewUpdated = false;
            viewChanged();
        }
        //xcb_generic_event_t *event;
        //while ((event = xcb_poll_for_event(connection)))
        //{
        //    handleEvent(event);
        //    free(event);
        //}
        render();
        frameCounter++;
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = tDiff / 1000.0f;
        camera.update(frameTimer);
        if (camera.moving())
        {
            viewUpdated = true;
        }
        // Convert to clamped timer value
        if (!paused)
        {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
                timer -= 1.0f;
            }
        }
        float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
        if (fpsTimer > 1000.0f)
        {
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
        }
        updateOverlay();
    //}
}

void VulkanBasicEngine::afterRender(){
    if (device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device);
    }
}
