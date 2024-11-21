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
    m_ViewProjectionConstantBuffer(),
    m_TDXShaderManager()
{
}

TDXRenderDevice::~TDXRenderDevice()
{
}

bool TDXRenderDevice::Initizialize(HWND hwnd, int width, int height)
{
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

    m_TDXTextureManager.CreateSampler(m_pDevice.Get(), m_pDeviceContext.Get());

    m_IsRunning = true;

    return true;
}

void TDXRenderDevice::BeginFrame(float r, float g, float b, float a) {
    float clearColor[] = { r, g, b, a };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_ViewProjectionConstantBuffer = {
        DirectX::XMMatrixTranspose(m_ViewMatrix),
        DirectX::XMMatrixTranspose(m_ProjectionMatrix)
    };

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(m_pViewProjectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_ViewProjectionConstantBuffer, sizeof(ViewProjectionConstantBuffer));
    m_pDeviceContext->Unmap(m_pViewProjectionBuffer.Get(), 0);
}

void TDXRenderDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void TDXRenderDevice::Draw(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation)
{
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(numIndices, startIndexLocation, baseVertexLocation);
}

void TDXRenderDevice::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void TDXRenderDevice::SetViewMatrix(Eigen::Matrix4d eigenMatrix)
{
    m_ViewMatrix = DirectX::XMMATRIX(
        eigenMatrix(0, 0), eigenMatrix(1, 0), eigenMatrix(2, 0), eigenMatrix(3, 0),
        eigenMatrix(0, 1), eigenMatrix(1, 1), eigenMatrix(2, 1), eigenMatrix(3, 1),
        eigenMatrix(0, 2), eigenMatrix(1, 2), eigenMatrix(2, 2), eigenMatrix(3, 2),
        eigenMatrix(0, 3), eigenMatrix(1, 3), eigenMatrix(2, 3), eigenMatrix(3, 3)
    );

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

    m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

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
    D3D11_BUFFER_DESC transformBufferDesc = {};
    transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    transformBufferDesc.ByteWidth = sizeof(ViewProjectionConstantBuffer);
    transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HRESULT hr = m_pDevice->CreateBuffer(&transformBufferDesc, nullptr, &m_pViewProjectionBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create TransformBuffer.");
    }

    m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pViewProjectionBuffer.GetAddressOf());
}

void TDXRenderDevice::AddShaders()
{
    m_TDXShaderManager.AddVertexShader("mesh", L"..\\TD3D\\VertexShader.cso",
        MESH_INPUT_LAYOUT, sizeof(MESH_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader("mesh", L"..\\TD3D\\PixelShader.cso", m_pDevice.Get());

    m_TDXShaderManager.AddVertexShader("skybox", L"..\\TD3D\\SkyboxVertexShader.cso",
        SKYBOX_INPUT_LAYOUT, sizeof(SKYBOX_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader("skybox", L"..\\TD3D\\SkyboxPixelShader.cso", m_pDevice.Get());
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
