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

bool ImportModel(const std::string& name, const std::string& filePath, TVertexGeometry*& vertices, unsigned int*& indices, unsigned int& numVertices, unsigned int& numIndices)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if (!scene || !scene->HasMeshes()) {
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    numVertices = mesh->mNumVertices;
    numIndices = mesh->mNumFaces * 3;

    vertices = new TVertexGeometry[numVertices];
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

    if (!(scene->mTextures == nullptr || scene->mNumTextures == 0)) {
        aiTexture* texture = scene->mTextures[0];

        if (texture->mHeight == 0) {
            unsigned char* compressedData = reinterpret_cast<unsigned char*>(texture->pcData);
            size_t dataSize = texture->mWidth;

            renderDevice->AddTexture(name, compressedData, dataSize);
        }
        else {
            unsigned char* rawData = reinterpret_cast<unsigned char*>(texture->pcData);
            int width = texture->mWidth;
            int height = texture->mHeight;

            renderDevice->AddTexture(name, rawData, width, height);
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

    TVertexGeometry* vertices = nullptr;
    unsigned int* indices = nullptr;
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    TVertexGeometry* terrainVert = nullptr;
    unsigned int* terrainInd = nullptr;
    unsigned int numTerrainVert = 0;
    unsigned int numTerrainInd = 0;

    ImportModel("model", "Models/model.fbx", vertices, indices, numVertices, numIndices);
    ImportModel("terrain", "Models/terrain.fbx", terrainVert, terrainInd, numTerrainVert, numTerrainInd);

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

    TVertexSprite verticesSpriteTexture[] = {
        { { 1.0f, 0.90f }, { 1.0f, 1.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 0.3f, 0.90f }, { 0.0f, 1.0f } },
        { { 0.3f, 1.0f }, { 0.0f, 0.0f } }
    };

    camera.SetPosition(2.0f, 2.0f, -2.0f);
    renderDevice->SetViewMatrix(camera.GetViewMatrix());

    Light dirLight = {};
    dirLight.Type = 0;
    dirLight.Direction = { 0.4f, 0.8f, 0.5f };
    dirLight.Color = { 1.0f, 1.0f, 1.0f };
    dirLight.Intensity = 1.0f;

    Light pointLight = {};
    pointLight.Type = 1;
    pointLight.Position = { 0.0f, 5.0f, 0.0f };
    pointLight.Color = { 1.0f, 0.5f, 0.5f };
    pointLight.Intensity = 20.0f;
    pointLight.Range = 100.0f;

    Light spotLight = {};
    spotLight.Type = 2;
    spotLight.Position = { 0.0f, 5.0f, 0.0f };
    spotLight.Direction = { 0.0f, -1.0f, 0.0f };
    spotLight.Color = { 0.5f, 0.5f, 1.0f };
    spotLight.Intensity = 30.0f;
    spotLight.Range = 150.0f;
    spotLight.SpotAngle = 30.0f;

    std::vector<Light> lights;
    lights.push_back(dirLight);
    //lights.push_back(pointLight);
    //lights.push_back(spotLight);

    renderDevice->SetAmbientLight(0.3f, 0.3f, 0.3f);
    renderDevice->SetLights(lights);

    renderDevice->AddTexture("crate", "Textures/crate.jpg");
    renderDevice->AddTexture("white", 120, 120, 120, 255);
    renderDevice->AddCubeMapTexture("skybox", "Textures/Skybox/Weltraum", ".png");

    renderDevice->CreateStaticVertexBuffer("VertexBufferGeometry", vertices, numVertices, sizeof(TVertexGeometry));
    renderDevice->CreateStaticIndexBuffer("IndexBufferGeometry", indices, numIndices);

    renderDevice->CreateStaticVertexBuffer("VertexBufferGeometryTerrain", terrainVert, numTerrainVert, sizeof(TVertexGeometry));
    renderDevice->CreateStaticIndexBuffer("IndexBufferGeometryTerrain", terrainInd, numTerrainInd);

    renderDevice->CreateStaticVertexBuffer("VertexBufferSkybox", verticesSkybox, sizeof(verticesSkybox) / sizeof(TVertexSkybox), sizeof(TVertexSkybox));
    renderDevice->CreateStaticIndexBuffer("IndexBufferSkybox", indicesSkybox, sizeof(indicesSkybox) / sizeof(unsigned int));

    renderDevice->CreateStaticVertexBuffer("VertexBufferSprite", verticesSpriteTexture, sizeof(verticesSpriteTexture) / sizeof(TVertexSprite), sizeof(TVertexSprite));

    auto start = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    std::wstring fpsText;

    Eigen::Matrix4f rotationFix = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f scaling = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f transformation;

    float angle = -PI / 2.0f;

    rotationFix(1, 1) = cos(angle);
    rotationFix(1, 2) = -sin(angle);
    rotationFix(2, 1) = sin(angle);
    rotationFix(2, 2) = cos(angle);

    scaling(0, 0) = 10.0f;
    scaling(1, 1) = 10.0f;
    scaling(2, 2) = 10.0f;

    translation(2, 3) = -1.5f;

    transformation = scaling * rotationFix * translation;

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

            renderDevice->BindVertexShader(GEOMETRY_SHADER);
            renderDevice->BindPixelShader(GEOMETRY_SHADER);

            renderDevice->BindTexture("crate");

            renderDevice->BindVertexBuffer("VertexBufferGeometry", sizeof(TVertexGeometry), 0);
            renderDevice->BindIndexBuffer("IndexBufferGeometry");

            renderDevice->SetWorldMatrix(rotationFix);

            renderDevice->Draw(numIndices, 0, 0);

            renderDevice->BindVertexBuffer("VertexBufferGeometryTerrain", sizeof(TVertexGeometry), 0);
            renderDevice->BindIndexBuffer("IndexBufferGeometryTerrain");

            renderDevice->SetWorldMatrix(transformation);

            renderDevice->BindGeometryShader(NORMAL_SHADER);
            renderDevice->BindTexture("white");

            renderDevice->Draw(numTerrainInd, 0, 0);

            renderDevice->UnbindGeometryShader(NORMAL_SHADER);

            renderDevice->BindTexture("crate");
            renderDevice->Draw(numTerrainInd, 0, 0);

            renderDevice->SetWorldMatrix(Eigen::Matrix4f::Identity());

            frameCount++;

            if (fpsTimer.GetMilisecondsElapsed() >= 1000.0f) {
                float fps = frameCount / (fpsTimer.GetMilisecondsElapsed() / 1000.0f);

                fpsText = L"FPS: " + std::to_wstring(fps);

                fpsTimer.Restart();
                frameCount = 0;
            }

            renderDevice->RenderText(fpsText.c_str(), 0, 0);

            renderDevice->BindVertexShader(SPRITE_SHADER);
            renderDevice->BindPixelShader(SPRITE_SHADER);

            renderDevice->BindTexture("white");
            renderDevice->BindVertexBuffer("VertexBufferSprite", sizeof(TVertexSprite), 0);

            renderDevice->DrawSprite(4, 0);

            renderDevice->EndFrame();
        }
    }

    renderer.Release();

    delete[] vertices;
    delete[] indices;

    delete[] terrainVert;
    delete[] terrainInd;

    return 0;
}
