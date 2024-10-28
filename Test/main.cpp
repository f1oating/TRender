#include <windows.h>
#include "TRenderer.h"
#include "T3DMath.h"

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

    renderDevice->Init(hwnd, 800, 600, true);

    TVertex vertices[] =
    {
        { 0.0f,0.5f,255,0,0,0 },
        { 0.5f,-0.5f,0,255,0,0 },
        { -0.5f,-0.5f,0,0,255,0 },
        { -0.3f,0.3f,0,255,0,0 },
        { 0.3f,0.3f,0,0,255,0 },
        { 0.0f,-0.8f,255,0,0,0 },
    };

    unsigned short indices[] =
    {
        0,1,2,
        0,2,3,
        0,4,1,
        2,1,5,
    };

    renderDevice->LoadMesh(vertices, sizeof(vertices) / sizeof(TVertex), indices, sizeof(indices) / sizeof(unsigned short));

    // Main message loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        // Process messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            renderDevice->BeginRendering();

            renderDevice->EndRendering();
        }
    }

    return 0;
}
