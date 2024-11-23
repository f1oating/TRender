#include "TDXRenderDevice.h"

#include <stdexcept>
#include <d3dcompiler.h>

TDXRenderDevice::TDXRenderDevice() :
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr),
	m_pDepthStencilBuffer(nullptr),
    m_pSpriteBatch(nullptr),
    m_pSpriteFont(nullptr),
    m_ViewMatrixCBS(),
    m_ProjectionMatrixCBS(),
    m_TDXShaderManager(),
    m_TDXTextureManager(),
    m_TDXFeatureController(),
    m_TDXBufferManager()
{
    m_ViewMatrix = DirectX::XMMatrixIdentity();
    m_ProjectionMatrix = DirectX::XMMatrixIdentity();
}

TDXRenderDevice::~TDXRenderDevice()
{
}

bool TDXRenderDevice::Initizialize(HWND hwnd, int width, int height)
{
    hwnd = hwnd;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0,
        D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Direct3D device and swap chain.");
    }

    CreateBuffers();
    OnResize(width, height);
    AddShaders();

    m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceContext.Get());
    m_pSpriteFont = std::make_unique<DirectX::SpriteFont>(m_pDevice.Get(), L"Fonts\\comic_sans_ms_16.spritefont");

    m_TDXTextureManager.CreateSampler("default", m_pDevice.Get(), m_pDeviceContext.Get());
    m_TDXTextureManager.BindSampler("default", m_pDeviceContext.Get());

    m_IsRunning = true;

    return true;
}

void TDXRenderDevice::BeginFrame(float r, float g, float b, float a) {
    float clearColor[] = { r, g, b, a };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void TDXRenderDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void TDXRenderDevice::Draw(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation)
{
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(numIndices, startIndexLocation, baseVertexLocation);
}

void TDXRenderDevice::DrawSprite(unsigned short numVertices, unsigned short startVertexLocation)
{
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SetRasterizerCulling(false);

    m_pDeviceContext->Draw(numVertices, startVertexLocation);

    SetRasterizerCulling(true);
}

void TDXRenderDevice::RenderText(const wchar_t* text, float x, float y)
{
    m_pSpriteBatch->Begin();
    m_pSpriteFont->DrawString(m_pSpriteBatch.get(), text, DirectX::XMFLOAT2(x, y));
    m_pSpriteBatch->End();

    m_TDXBufferManager.BindConstantBuffer(PROJECTION_MATRIX_CONSTANT_BUFFER, 0, m_pDeviceContext.Get());
    m_TDXBufferManager.BindConstantBuffer(VIEW_MATRIX_CONSTANT_BUFFER, 1, m_pDeviceContext.Get());
    m_TDXTextureManager.BindSampler("default", m_pDeviceContext.Get());
}

void TDXRenderDevice::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

    m_ProjectionMatrixCBS = { DirectX::XMMatrixTranspose(m_ProjectionMatrix) };

    m_TDXBufferManager.UpdateDynamicConstantBuffer(PROJECTION_MATRIX_CONSTANT_BUFFER, &m_ProjectionMatrixCBS, sizeof(MatrixCBS), m_pDeviceContext.Get());
}

void TDXRenderDevice::SetViewMatrix(const Eigen::Matrix4d& matrix)
{
    m_ViewMatrix = DirectX::XMMATRIX(
        static_cast<float>(matrix(0, 0)), static_cast<float>(matrix(1, 0)), static_cast<float>(matrix(2, 0)), static_cast<float>(matrix(3, 0)),
        static_cast<float>(matrix(0, 1)), static_cast<float>(matrix(1, 1)), static_cast<float>(matrix(2, 1)), static_cast<float>(matrix(3, 1)),
        static_cast<float>(matrix(0, 2)), static_cast<float>(matrix(1, 2)), static_cast<float>(matrix(2, 2)), static_cast<float>(matrix(3, 2)),
        static_cast<float>(matrix(0, 3)), static_cast<float>(matrix(1, 3)), static_cast<float>(matrix(2, 3)), static_cast<float>(matrix(3, 3))
    );

    m_ViewMatrixCBS = { DirectX::XMMatrixTranspose(m_ViewMatrix) };

    m_TDXBufferManager.UpdateDynamicConstantBuffer(VIEW_MATRIX_CONSTANT_BUFFER, &m_ViewMatrixCBS, sizeof(MatrixCBS), m_pDeviceContext.Get());
}

void TDXRenderDevice::AddTexture(std::string name, std::string path)
{
    m_TDXTextureManager.AddTexture(name, path, m_pDevice.Get());
}

void TDXRenderDevice::AddCubeMapTexture(std::string name, std::string path, std::string ext)
{
    m_TDXTextureManager.AddCubeMapTexture(name, path, ext, m_pDevice.Get());
}

void TDXRenderDevice::BindTexture(std::string name)
{
    m_TDXTextureManager.BindTexture(name, m_pDeviceContext.Get());
}

void TDXRenderDevice::BindVertexShader(std::string name)
{
    m_TDXShaderManager.BindVertexShader(name, m_pDeviceContext.Get());
}

void TDXRenderDevice::BindPixelShader(std::string name)
{
    m_TDXShaderManager.BindPixelShader(name, m_pDeviceContext.Get());
}

void TDXRenderDevice::CreateStaticVertexBuffer(std::string name, void* vertices, unsigned short numVertices, unsigned short vertexSize)
{
    m_TDXBufferManager.CreateStaticVertexBuffer(name, vertices, numVertices, vertexSize, m_pDevice.Get());
}

void TDXRenderDevice::CreateStaticIndexBuffer(std::string name, unsigned short* indices, unsigned short numIndices)
{
    m_TDXBufferManager.CreateStaticIndexBuffer(name, indices, numIndices, m_pDevice.Get());
}

void TDXRenderDevice::UpdateStaticVertexBuffer(std::string name, void* vertices)
{
    m_TDXBufferManager.UpdateStaticVertexBuffer(name, vertices, m_pDeviceContext.Get());
}

void TDXRenderDevice::UpdateStaticIndexBuffer(std::string name, unsigned short* indices)
{
    m_TDXBufferManager.UpdateStaticIndexBuffer(name, indices, m_pDeviceContext.Get());
}

void TDXRenderDevice::BindVertexBuffer(std::string vertexName, UINT stride, UINT offset)
{
    m_TDXBufferManager.BindVertexBuffer(vertexName, stride, offset, m_pDeviceContext.Get());
}

void TDXRenderDevice::BindIndexBuffer(std::string indexName)
{
    m_TDXBufferManager.BindIndexBuffer(indexName, m_pDeviceContext.Get());
}

void TDXRenderDevice::DeleteBuffer(std::string name)
{
    m_TDXBufferManager.DeleteBuffer(name);
}

void TDXRenderDevice::SetDepthStencilComparison(bool flag)
{
    m_TDXFeatureController.ChangeDepthStencilComparison(flag, m_pDevice.Get(), m_pDeviceContext.Get());

}

void TDXRenderDevice::SetRasterizerCulling(bool flag)
{
    m_TDXFeatureController.ChangeRasterizerCulling(flag, m_pDevice.Get(), m_pDeviceContext.Get());
}

bool TDXRenderDevice::OnResize(int width, int height)
{
    m_pRenderTargetView.Reset();
    m_pDepthStencilBuffer.Reset();
    m_pDepthStencilView.Reset();

    SetProjectionValues(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

    HRESULT hr = m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to resize back buffer.");
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (&backBuffer));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer.");
    }

    hr = m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_pRenderTargetView);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view.");
    }

    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil buffer.");
    }

    hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, &m_pDepthStencilView);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create depth stencil view.");
    }

    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_pDeviceContext->RSSetViewports(1, &viewport);

    return true;
}

bool TDXRenderDevice::IsRunning()
{
    return m_IsRunning;
}

void TDXRenderDevice::CreateBuffers()
{
    m_TDXBufferManager.CreateDynamicConstantBuffer(PROJECTION_MATRIX_CONSTANT_BUFFER, sizeof(MatrixCBS), m_pDevice.Get());
    m_TDXBufferManager.BindConstantBuffer(PROJECTION_MATRIX_CONSTANT_BUFFER, 0, m_pDeviceContext.Get());

    m_TDXBufferManager.CreateDynamicConstantBuffer(VIEW_MATRIX_CONSTANT_BUFFER, sizeof(MatrixCBS), m_pDevice.Get());
    m_TDXBufferManager.BindConstantBuffer(VIEW_MATRIX_CONSTANT_BUFFER, 1, m_pDeviceContext.Get());
}

void TDXRenderDevice::AddShaders()
{
    m_TDXShaderManager.AddVertexShader(MESH_SHADER, L"..\\TD3D\\MeshVertexShader.cso",
        MESH_INPUT_LAYOUT, sizeof(MESH_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader(MESH_SHADER, L"..\\TD3D\\MeshPixelShader.cso", m_pDevice.Get());

    m_TDXShaderManager.AddVertexShader(SKYBOX_SHADER, L"..\\TD3D\\SkyboxVertexShader.cso",
        SKYBOX_INPUT_LAYOUT, sizeof(SKYBOX_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader(SKYBOX_SHADER, L"..\\TD3D\\SkyboxPixelShader.cso", m_pDevice.Get());

    m_TDXShaderManager.AddVertexShader(SPRITE_SHADER_TEXTURE, L"..\\TD3D\\SpriteTextureVertexShader.cso",
        SPRITE_INPUT_LAYOUT, sizeof(SPRITE_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader(SPRITE_SHADER_TEXTURE, L"..\\TD3D\\SpriteTexturePixelShader.cso", m_pDevice.Get());
}

HRESULT CreateRenderDevice(TDXRenderDevice** pDevice) {
    if (!*pDevice) {
        *pDevice = new TDXRenderDevice();
        return S_OK;
    }
    return 0x82000001;
}

HRESULT ReleaseRenderDevice(TDXRenderDevice** pDevice) {
    if (!*pDevice) {
        return 0x82000001;
    }
    delete* pDevice;
    *pDevice = nullptr;
    return S_OK;
}
