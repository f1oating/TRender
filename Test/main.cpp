#include <windows.h>
#include "TRenderer.h"
#include "TInput.h"

TRenderDevice* renderDevice;

void SimpleMoving(TInput* input)
{
    input->Update();

    if (input->IsKeyDown('W')) {
        renderDevice->AdjustPosition(0.0f, 0.0f, 0.005f);
    }

    if (input->IsKeyDown('S')) {
        renderDevice->AdjustPosition(0.0f, 0.0f, -0.005f);
    }

    if (input->IsKeyDown('A')) {
        renderDevice->AdjustPosition(-0.005f, 0.0f, 0.0f);
    }

    if (input->IsKeyDown('D')) {
        renderDevice->AdjustPosition(0.005f, 0.0f, 0.0f);
    }

    long deltaX = input->GetDeltaX();
    long deltaY = input->GetDeltaY();

    if (deltaX > 0) {
        renderDevice->AdjustRotation(0.0f, 0.01f, 0.0f);
    }
    else if (deltaX < 0) {
        renderDevice->AdjustRotation(0.0f, -0.01f, 0.0f);
    }

    if (deltaY > 0) {
        renderDevice->AdjustRotation(0.01f, 0.0f, 0.0f);
    }
    else if (deltaY < 0) {
        renderDevice->AdjustRotation(-0.01f, 0.0f, 0.0f);
    }
}

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
    TInput input(hwnd);

    // Create the rendering device
    HRESULT hr = renderer.CreateDevice("Direct3D");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to create renderer device.", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    renderDevice = renderer.GetDevice();

    renderDevice->Initizialize(hwnd, 800, 600);

    TVertexPT vertices[] = {
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}},
    {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f}},

    {{ 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}},
    {{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}},

    {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}},

    {{ 1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f}},

    {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}},

    {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}}
    };

    unsigned short indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    renderDevice->SetViewPosition(2.0f, 2.0f, -2.0f);
    renderDevice->AddTexture("crate", "Textures/crate.jpg");
    renderDevice->AddCubeMapTexture("skybox", "Textures/Skybox/Weltraum", ".png");
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
            SimpleMoving(&input);

            renderDevice->BeginFrame(0.1f, 0.1f, 0.1f, 1.0f);

            renderDevice->SetRasterizerCulling(false);
            renderDevice->SetDepthStencilComparison(false);

            renderDevice->BindVertexShader("skybox");
            renderDevice->BindPixelShader("skybox");

            renderDevice->BindTexture("skybox");
            renderDevice->DrawPT(36, 0, 0);

            renderDevice->SetRasterizerCulling(true);
            renderDevice->SetDepthStencilComparison(true);

            renderDevice->BindVertexShader("mesh");
            renderDevice->BindPixelShader("mesh");

            renderDevice->BindTexture("crate");
            renderDevice->DrawPT(36, 0, 0);

            renderDevice->EndFrame();
        }
    }

    renderer.Release();

    return 0;
}
