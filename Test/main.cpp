#define NOMINMAX

#include <windows.h>
#include <iostream>
#include <chrono>
#include "TRenderer.h"
#include "TInput.h"
#include "TestCamera.h"
#include "TestTimer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

TRenderDevice* renderDevice;
TestCamera camera;
TestTimer timer;
TestTimer fpsTimer;
int frameCount = 0;
std::wstring fpsText;

bool ImportModel(const std::string& filePath, TVertexMesh*& vertices, unsigned int*& indices, unsigned int& numVertices, unsigned int& numIndices)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->HasMeshes()) {
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    numVertices = mesh->mNumVertices;
    numIndices = mesh->mNumFaces * 3;

    vertices = new TVertexMesh[numVertices];
    indices = new unsigned int[numIndices];

    for (unsigned int i = 0; i < numVertices; ++i) {
        vertices[i].pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        vertices[i].normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (mesh->HasTangentsAndBitangents()) {
            vertices[i].tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
        }
        else {
            vertices[i].tangent = { 0.0f, 0.0f, 0.0f };
        }

        if (mesh->HasTextureCoords(0)) {
            vertices[i].tex = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else {
            vertices[i].tex = { 0.0f, 0.0f };
        }
    }

    unsigned int index = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices[index++] = face.mIndices[j];
        }
    }

    return true;
}

void SimpleMoving(TInput* input)
{
    input->Update();
    
    if (timer.GetMilisecondsElapsed() >= 10.0)
    {

        timer.Restart();

        if (input->IsKeyDown('W')) {
            camera.MoveForward(0.05f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

        if (input->IsKeyDown('S')) {
            camera.MoveBackward(0.05f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

        if (input->IsKeyDown('A')) {
            camera.MoveLeft(0.05f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

        if (input->IsKeyDown('D')) {
            camera.MoveRight(0.05f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

        long deltaX = input->GetDeltaX();
        long deltaY = input->GetDeltaY();

        if (deltaX > 0) {
            camera.AdjustRotation(0.0f, 0.05f, 0.0f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }
        else if (deltaX < 0) {
            camera.AdjustRotation(0.0f, -0.05f, 0.0f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

        if (deltaY > 0) {
            camera.AdjustRotation(0.05f, 0.0f, 0.0f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }
        else if (deltaY < 0) {
            camera.AdjustRotation(-0.05f, 0.0f, 0.0f);
            renderDevice->SetViewMatrix(camera.GetViewMatrix());
        }

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
    timer.Start();
    fpsTimer.Start();

    // Create the rendering device
    HRESULT hr = renderer.CreateDevice("Direct3D");
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to create renderer device.", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    renderDevice = renderer.GetDevice();

    renderDevice->Initizialize(hwnd, 800, 600);

    TVertexMesh* vertices = nullptr;
    unsigned int* indices = nullptr;
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    ImportModel("Models/model.fbx", vertices, indices, numVertices, numIndices);

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

    unsigned int indicesSkybox[] = {
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

    renderDevice->CreateStaticVertexBuffer("VertexBufferMesh", vertices, numVertices, sizeof(TVertexMesh));
    renderDevice->CreateStaticIndexBuffer("IndexBufferMesh", indices, numIndices);

    renderDevice->CreateStaticVertexBuffer("VertexBufferSkybox", verticesSkybox, sizeof(verticesSkybox) / sizeof(TVertexSkybox), sizeof(TVertexSkybox));
    renderDevice->CreateStaticIndexBuffer("IndexBufferSkybox", indicesSkybox, sizeof(indicesSkybox) / sizeof(unsigned int));

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

            renderDevice->BindVertexBuffer("VertexBufferMesh", sizeof(TVertexMesh), 0);
            renderDevice->BindIndexBuffer("IndexBufferMesh");

            renderDevice->Draw(numIndices, 0, 0);

            frameCount++;

            if (fpsTimer.GetMilisecondsElapsed() >= 1000.0f) {
                float fps = frameCount / (fpsTimer.GetMilisecondsElapsed() / 1000.0f);

                fpsText = L"FPS: " + std::to_wstring(fps);

                fpsTimer.Restart();
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

            renderDevice->SetBlendState(true);

            renderDevice->DrawSprite(3, 0);

            renderDevice->SetBlendState(false);

            renderDevice->EndFrame();
        }
    }

    renderer.Release();

    return 0;
}
