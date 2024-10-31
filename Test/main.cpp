#include <windows.h>
#include "TRenderer.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Register the window class
    const TCHAR CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Test TRenderer",               // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, // Size and position
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                     // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Create a renderer
    TRenderer renderer(hInstance);

    // Create the rendering device
    HRESULT hr = renderer.CreateDevice("Direct3D");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to create renderer device.", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    TRenderDevice* renderDevice = renderer.GetDevice();

    renderDevice->Initizialize(hwnd, 800, 600);

    TVertexColor vertices[] = {
    {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{ -0.3f,  0.3f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    {{0.3f, 0.3f,  0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
    {{ 0.0f, -0.8f,  0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
    };

    unsigned short indices[] = {
        0,1,2,
        0,2,3,
        0,4,1,
        2,1,5
    };

    // Main message loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        // Process messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            renderDevice->BeginFrame(0.1f, 0.1f, 0.1f, 1.0f);

            renderDevice->DrawIndexed(vertices, sizeof(vertices) / sizeof(TVertexColor), indices, sizeof(indices) / sizeof(unsigned short));

            renderDevice->EndFrame();
        }
    }

    return 0;
}
