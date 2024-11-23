#include <windows.h>
#include <iostream>
#include <chrono>
#include "TRenderer.h"
#include "TInput.h"
#include "TestCamera.h"

TRenderDevice* renderDevice;
TestCamera camera;

void SimpleMoving(TInput* input)
{
    input->Update();

    if (input->IsKeyDown('W')) {
        camera.AdjustPosition(0.0f, 0.0f, 0.005f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (input->IsKeyDown('S')) {
        camera.AdjustPosition(0.0f, 0.0f, -0.005f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (input->IsKeyDown('A')) {
        camera.AdjustPosition(-0.005f, 0.0f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (input->IsKeyDown('D')) {
        camera.AdjustPosition(0.005f, 0.0f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (input->IsKeyDown('U')) {
        camera.AdjustPosition(0.0f, 0.005f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (input->IsKeyDown('B')) {
        camera.AdjustPosition(0.0f, -0.005f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    long deltaX = input->GetDeltaX();
    long deltaY = input->GetDeltaY();

    if (deltaX > 0) {
        camera.AdjustRotation(0.0f, 0.01f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }
    else if (deltaX < 0) {
        camera.AdjustRotation(0.0f, -0.01f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }

    if (deltaY > 0) {
        camera.AdjustRotation(0.01f, 0.0f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
    }
    else if (deltaY < 0) {
        camera.AdjustRotation(-0.01f, 0.0f, 0.0f);
        renderDevice->SetViewMatrix(camera.GetViewMatrix());
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

    TVertexSkybox verticesSkybox[] = {
    {{-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}},
    {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}},

    // Back (-Z)
    {{ 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}},

    // Left (-X)
    {{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}},

    // Right (+X)
    {{ 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}},
    {{ 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}},
    {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}},

    // Top (+Y)
    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}},
    {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}},

    // Bottom (-Y)
    {{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},
    {{ 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}},
    {{-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}}
    };

    unsigned short indicesSkybox[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    TVertexSpriteTexture verticesSpriteTexture[] = {
        { { 1.0f, 0.90f }, { 1.0f, 1.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 0.3f, 1.0f }, { 0.0f, 0.0f } },

        { { 0.3f, 0.9f }, { 0.0f, 1.0f } },
        { { 1.0f, 0.9f }, { 1.0f, 1.0f } },
        { { 0.3f, 1.0f }, { 0.0f, 0.0f } }
    };

    TVertexSpriteColor verticesSpriteColor[] = {
        { { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.2f } },
        { { 0.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.2f } },
        { { 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.2f } },
    };

    camera.SetPosition(2.0f, 2.0f, -2.0f);

    renderDevice->AddTexture("crate", "Textures/crate.jpg");
    renderDevice->AddCubeMapTexture("skybox", "Textures/Skybox/Weltraum", ".png");

    renderDevice->CreateStaticVertexBuffer("VertexBufferMesh", vertices, sizeof(vertices) / sizeof(TVertexPT), sizeof(TVertexPT));
    renderDevice->CreateStaticIndexBuffer("IndexBufferMesh", indices, sizeof(indices) / sizeof(unsigned short));

    renderDevice->CreateStaticVertexBuffer("VertexBufferSkybox", verticesSkybox, sizeof(verticesSkybox) / sizeof(TVertexSkybox), sizeof(TVertexSkybox));
    renderDevice->CreateStaticIndexBuffer("IndexBufferSkybox", indicesSkybox, sizeof(indicesSkybox) / sizeof(unsigned short));

    renderDevice->CreateStaticVertexBuffer("VertexBufferSprite", verticesSpriteTexture, sizeof(verticesSpriteTexture) / sizeof(TVertexSpriteTexture), sizeof(TVertexSpriteTexture));
    renderDevice->CreateDynamicVertexBuffer("VertexBufferSpriteColor", verticesSpriteColor, sizeof(verticesSpriteColor) / sizeof(TVertexSpriteColor), sizeof(TVertexSpriteColor));

    auto start = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    std::wstring fpsText;

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

            renderDevice->BindVertexShader(SKYBOX_SHADER);
            renderDevice->BindPixelShader(SKYBOX_SHADER);

            renderDevice->BindTexture("skybox");

            renderDevice->BindVertexBuffer("VertexBufferSkybox", sizeof(TVertexSkybox), 0);
            renderDevice->BindIndexBuffer("IndexBufferSkybox");

            renderDevice->Draw(36, 0, 0);

            renderDevice->SetRasterizerCulling(true);
            renderDevice->SetDepthStencilComparison(true);

            renderDevice->BindVertexShader(MESH_SHADER);
            renderDevice->BindPixelShader(MESH_SHADER);

            renderDevice->BindTexture("crate");

            renderDevice->BindVertexBuffer("VertexBufferMesh", sizeof(TVertexPT), 0);
            renderDevice->BindIndexBuffer("IndexBufferMesh");

            renderDevice->Draw(36, 0, 0);

            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - start);

            frameCount++;

            if (elapsed.count() >= 1.0f) {
                float fps = frameCount / elapsed.count();

                fpsText = L"FPS: " + std::to_wstring(fps);

                start = now;
                frameCount = 0;
            }

            renderDevice->RenderText(fpsText.c_str(), 0, 0);

            renderDevice->BindVertexShader(SPRITE_SHADER_TEXTURE);
            renderDevice->BindPixelShader(SPRITE_SHADER_TEXTURE);

            renderDevice->BindTexture("crate");
            renderDevice->BindVertexBuffer("VertexBufferSprite", sizeof(TVertexSpriteTexture), 0);

            renderDevice->DrawSprite(6, 0);

            renderDevice->BindVertexShader(SPRITE_SHADER_COLOR);
            renderDevice->BindPixelShader(SPRITE_SHADER_COLOR);

            renderDevice->BindVertexBuffer("VertexBufferSpriteColor", sizeof(TVertexSpriteColor), 0);

            renderDevice->DrawSprite(3, 0);

            renderDevice->EndFrame();
        }
    }

    renderer.Release();

    return 0;
}
