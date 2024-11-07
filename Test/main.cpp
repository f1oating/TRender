#include <windows.h>
#include "TRenderer.h"

TRenderDevice* renderDevice;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (renderDevice) renderDevice->OnResize(LOWORD(lParam), HIWORD(lParam));
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

    renderDevice = renderer.GetDevice();

    renderDevice->Initizialize(hwnd, 800, 600);

    TVertexPT vertices[] = {
        {{-1.0f,-1.0f,-1.0f}, {0.0f, 1.0f}},
        {{1.0f,-1.0f,-1.0f}, {1.0f, 1.0f}},
        {{-1.0f,1.0f,-1.0f}, {0.0f, 0.0f}},
        {{1.0f,1.0f,-1.0f}, {1.0f, 0.0f}},
        {{-1.0f,-1.0f,1.0f}, {1.0f, 0.0f}},
        {{1.0f,-1.0f,1.0f}, {0.0f, 0.0f}},
        {{-1.0f,1.0f,1.0f}, {1.0f, 1.0f}},
        {{1.0f,1.0f,1.0f}, {0.0f, 1.0f}},

        {{-4.0f,-4.0f,-4.0f}, {0.0f, 1.0f}},
        {{-2.0f,-4.0f,-4.0f}, {1.0f, 1.0f}},
        {{-4.0f,-2.0f,-4.0f}, {0.0f, 0.0f}},
        {{-2.0f,-2.0f,-4.0f}, {1.0f, 0.0f}},
        {{-4.0f,-4.0f,-2.0f}, {1.0f, 0.0f}},
        {{-2.0f,-4.0f,-2.0f}, {0.0f, 0.0f}},
        {{-4.0f,-2.0f,-2.0f}, {1.0f, 1.0f}},
        {{-2.0f,-2.0f,-2.0f}, {0.0f, 1.0f}},
    };

    unsigned short indices[] = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4,

        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    renderDevice->SetViewMatrix({ 5.0f, 5.0f, -5.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f });
    renderDevice->AddTexture("crate", "crate.jpg");
    renderDevice->UpdatePTBuffer(vertices, sizeof(vertices) / sizeof(TVertexPT), indices, sizeof(indices) / sizeof(unsigned short));

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

            renderDevice->BindTexture("crate");
            renderDevice->DrawPT(36, 0, 0);
            renderDevice->DrawPT(36, 36, 8);

            renderDevice->EndFrame();
        }
    }

    renderer.Release();

    return 0;
}
