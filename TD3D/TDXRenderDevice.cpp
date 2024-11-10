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
    m_TDXShaderManager(),
    m_Camera()
{
    UpdateViewMatrix();
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
        DirectX::XMMatrixTranspose(m_Camera.viewMatrix),
        DirectX::XMMatrixTranspose(m_Camera.projectionMatrix)
    };

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(m_pViewProjectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_ViewProjectionConstantBuffer, sizeof(ViewProjectionConstantBuffer));
    m_pDeviceContext->Unmap(m_pViewProjectionBuffer.Get(), 0);
}

void TDXRenderDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void TDXRenderDevice::DrawPT(unsigned short numIndices, unsigned short startIndexLocation, unsigned short baseVertexLocation)
{
    const UINT stride = sizeof(TVertexPT);
    const UINT offset = 0u;

    m_pDeviceContext->IASetVertexBuffers(0, 1u, m_pStaticVertexBufferPT.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(m_pStaticIndexBufferPT.Get(), DXGI_FORMAT_R16_UINT, 0u);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(numIndices, startIndexLocation, baseVertexLocation);
}

void TDXRenderDevice::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    m_Camera.projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void TDXRenderDevice::SetViewPosition(float x, float y, float z)
{
    m_Camera.pos = DirectX::XMFLOAT3(x, y, z);
    m_Camera.posVector = DirectX::XMLoadFloat3(&m_Camera.pos);
    UpdateViewMatrix();
}

void TDXRenderDevice::AdjustPosition(float x, float y, float z)
{
    m_Camera.pos.x += x;
    m_Camera.pos.y += y;
    m_Camera.pos.z += z;
    m_Camera.posVector = DirectX::XMLoadFloat3(&m_Camera.pos);
    UpdateViewMatrix();
}

void TDXRenderDevice::SetRotation(float x, float y, float z)
{
    m_Camera.rot = DirectX::XMFLOAT3(x, y, z);
    m_Camera.rotVector = DirectX::XMLoadFloat3(&m_Camera.rot);
    UpdateViewMatrix();
}

void TDXRenderDevice::AdjustRotation(float x, float y, float z)
{
    m_Camera.rot.x += x;
    m_Camera.rot.y += y;
    m_Camera.rot.z += z;
    m_Camera.rotVector = DirectX::XMLoadFloat3(&m_Camera.rot);
    UpdateViewMatrix();
}

void TDXRenderDevice::SetLookAtPos(float x, float y, float z)
{
    if (x == m_Camera.pos.x && y == m_Camera.pos.y && z == m_Camera.pos.z)
        return;

    x = m_Camera.pos.x - x;
    y = m_Camera.pos.y - y;
    z = m_Camera.pos.z - z;

    float pitch = 0.0f;
    if (y != 0.0f)
    {
        const float distance = sqrt(x * x + z * z);
        pitch = atan(y / distance);
    }

    float yaw = 0.0f;
    if (x != 0.0f)
    {
        yaw = atan(x / z);
    }
    if (z > 0)
        yaw += DirectX::XM_PI;

    this->SetRotation(pitch, yaw, 0.0f);
}

void TDXRenderDevice::UpdatePTBuffer(TVertexPT* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices)
{
    if (m_pStaticVertexBufferPT) {
        m_pStaticVertexBufferPT->Release();
        m_pStaticVertexBufferPT = nullptr;
    }

    if (m_pStaticIndexBufferPT) {
        m_pStaticIndexBufferPT->Release();
        m_pStaticIndexBufferPT = nullptr;
    }

    D3D11_BUFFER_DESC staticVertexBufferPTDesc = {};
    staticVertexBufferPTDesc.Usage = D3D11_USAGE_DEFAULT;
    staticVertexBufferPTDesc.ByteWidth = sizeof(TVertexPT) * numVertices;
    staticVertexBufferPTDesc.StructureByteStride = sizeof(TVertexPT);
    staticVertexBufferPTDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    staticVertexBufferPTDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticVertexBufferPTInitData = {};
    staticVertexBufferPTInitData.pSysMem = vertices;
    HRESULT hr = m_pDevice->CreateBuffer(&staticVertexBufferPTDesc, &staticVertexBufferPTInitData, &m_pStaticVertexBufferPT);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static PT vertex buffer.");
    }

    D3D11_BUFFER_DESC staticIndexBufferPTDesc = {};
    staticIndexBufferPTDesc.Usage = D3D11_USAGE_DEFAULT;
    staticIndexBufferPTDesc.ByteWidth = sizeof(unsigned short) * numIndices;
    staticVertexBufferPTDesc.StructureByteStride = sizeof(unsigned short);
    staticIndexBufferPTDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    staticIndexBufferPTDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA staticIndexBufferPTInitData = {};
    staticIndexBufferPTInitData.pSysMem = indices;
    hr = m_pDevice->CreateBuffer(&staticIndexBufferPTDesc, &staticIndexBufferPTInitData, &m_pStaticIndexBufferPT);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create static PT index buffer.");
    }
}

void TDXRenderDevice::AddTexture(std::string name, std::string path)
{
    m_TDXTextureManager.AddTexture(name, path, m_pDevice.Get());
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

    m_Camera.projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

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
        MESH_INPUT_LAYOUT, sizeof(MESH_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.AddPixelShader("skybox", L"..\\TD3D\\SkyboxPixelShader.cso", m_pDevice.Get());
}

void TDXRenderDevice::UpdateViewMatrix()
{
    //Calculate camera rotation matrix
    DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Camera.rot.x, m_Camera.rot.y, m_Camera.rot.z);
    //Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
    DirectX::XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    //Adjust cam target to be offset by the camera's current position
    camTarget = DirectX::XMVectorAdd(camTarget, m_Camera.posVector);
    //Calculate up direction based on current rotation
    DirectX::XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
    //Rebuild view matrix
    m_Camera.viewMatrix = DirectX::XMMatrixLookAtLH(m_Camera.posVector, camTarget, upDir);
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
