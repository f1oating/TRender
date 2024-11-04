#include "TDXRenderDevice.h"

#include "TD3D.h"
#include <stdexcept>
#include <d3dcompiler.h>

TDXRenderDevice::TDXRenderDevice() :
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr),
	m_pDepthStencilBuffer(nullptr),
    m_LightsConstantBuffer(),
    m_AmbientLightConstantBuffer(),
    m_TransformConstantBuffer(),
    m_DirectLightConstantBuffer(),
    m_TDXShaderManager()
{
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_ViewMatrix = DirectX::XMMatrixLookAtLH(eye, at, up);
    m_ProjMatrix = DirectX::XMMatrixIdentity();
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

    m_TDXShaderManager.AddShaders("mesh", L"..\\TD3D\\VertexShader.cso", L"..\\TD3D\\PixelShader.cso", 
        MESH_INPUT_LAYOUT, sizeof(MESH_INPUT_LAYOUT) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pDevice.Get());
    m_TDXShaderManager.BindShaders("mesh", m_pDeviceContext.Get());

    m_IsRunning = true;

    return true;
}

void TDXRenderDevice::BeginFrame(float r, float g, float b, float a) {
    float clearColor[] = { r, g, b, a };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_TransformConstantBuffer = {
        DirectX::XMMatrixTranspose(m_ViewMatrix * m_ProjMatrix)
    };

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(m_pTransformBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_TransformConstantBuffer, sizeof(TransformConstantBuffer));
    m_pDeviceContext->Unmap(m_pTransformBuffer.Get(), 0);

    m_pDeviceContext->Map(m_pLightsBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_LightsConstantBuffer, sizeof(LightConstantBuffer));
    m_pDeviceContext->Unmap(m_pLightsBuffer.Get(), 0);

    m_pDeviceContext->Map(m_pDirectLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_DirectLightConstantBuffer, sizeof(DirectLightConstantBuffer));
    m_pDeviceContext->Unmap(m_pDirectLightBuffer.Get(), 0);
}

void TDXRenderDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void TDXRenderDevice::Draw(TVertexColor* vertices, unsigned short numVertices)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, vertices, sizeof(TVertexColor) * numVertices);
    m_pDeviceContext->Unmap(m_pVertexBuffer.Get(), 0);

    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    m_pDeviceContext->IASetVertexBuffers(0, 1u, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->Draw(numVertices, 0);
}

void TDXRenderDevice::Draw(TVertexColor* vertices, unsigned short numVertices, unsigned short* indices, unsigned short numIndices)
{
    D3D11_MAPPED_SUBRESOURCE vMappedResource;
    HRESULT hr = m_pDeviceContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &vMappedResource);
    memcpy(vMappedResource.pData, vertices, sizeof(TVertexColor) * numVertices);
    m_pDeviceContext->Unmap(m_pVertexBuffer.Get(), 0);

    D3D11_MAPPED_SUBRESOURCE iMappedResource;
    m_pDeviceContext->Map(m_pIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &iMappedResource);
    memcpy(iMappedResource.pData, indices, sizeof(unsigned short) * numIndices);
    m_pDeviceContext->Unmap(m_pIndexBuffer.Get(), 0);

    const UINT stride = sizeof(TVertexColor);
    const UINT offset = 0u;

    m_pDeviceContext->IASetVertexBuffers(0, 1u, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->DrawIndexed(numIndices, 0, 0);
}

void TDXRenderDevice::SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearZ, float farZ)
{
    m_ProjMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ);
}

void TDXRenderDevice::SetViewMatrix(TVector4 eye, TVector4 at, TVector4 up)
{
    DirectX::XMVECTOR deye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, eye.w);
    DirectX::XMVECTOR dat = DirectX::XMVectorSet(at.x, at.y, at.z, at.w);
    DirectX::XMVECTOR dup = DirectX::XMVectorSet(up.x, up.y, up.z, up.w);
    m_ViewMatrix = DirectX::XMMatrixLookAtLH(deye, dat, dup);
}

void TDXRenderDevice::SetAmbientLight(float r, float g, float b, float a)
{
    m_AmbientLightConstantBuffer = { { r, g, b, a } };
    m_pDeviceContext->UpdateSubresource(m_pAmbientLightBuffer.Get(), 0, nullptr, &m_AmbientLightConstantBuffer, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pAmbientLightBuffer.GetAddressOf());
}

void TDXRenderDevice::SetLights(LightConstantBuffer lights)
{
    m_LightsConstantBuffer = lights;
}

void TDXRenderDevice::SetDirectLight(TVector3 direction, float intencity, TVector3 color, float pad)
{
    m_DirectLightConstantBuffer = { direction, intencity, color, pad };
}

bool TDXRenderDevice::OnResize(int width, int height)
{
    m_pRenderTargetView.Reset();
    m_pDepthStencilBuffer.Reset();
    m_pDepthStencilView.Reset();

    float fieldOfView = DirectX::XM_PIDIV4;
    float aspectRatio = width / height;
    float nearZ = 0.1f;
    float farZ = 100.0f;
    m_ProjMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ);

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
    D3D11_BUFFER_DESC vbd = {};
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0u;
    vbd.ByteWidth = 16000;
    vbd.StructureByteStride = sizeof(TVertexColor);

    HRESULT hr = m_pDevice->CreateBuffer(&vbd, nullptr, &m_pVertexBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create VertexBuffer.");
    }

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = 16000;
    ibd.StructureByteStride = sizeof(unsigned short);

    hr = m_pDevice->CreateBuffer(&ibd, nullptr, &m_pIndexBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create IndexBuffer.");
    }

    D3D11_BUFFER_DESC ambientLightBufferDesc = {};
    ambientLightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    ambientLightBufferDesc.ByteWidth = sizeof(AmbientLightConstantBuffer);
    ambientLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ambientLightBufferDesc.CPUAccessFlags = 0;
    hr = m_pDevice->CreateBuffer(&ambientLightBufferDesc, nullptr, &m_pAmbientLightBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create AmbientLightBuffer.");
    }

    m_AmbientLightConstantBuffer = { { 0.2f, 0.2f, 0.2f, 1.0f } };
    m_pDeviceContext->UpdateSubresource(m_pAmbientLightBuffer.Get(), 0, nullptr, &m_AmbientLightConstantBuffer, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pAmbientLightBuffer.GetAddressOf());

    D3D11_BUFFER_DESC lightsBufferDesc = {};
    lightsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightsBufferDesc.ByteWidth = sizeof(LightConstantBuffer);
    lightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_pDevice->CreateBuffer(&lightsBufferDesc, nullptr, &m_pLightsBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create LightsBuffer.");
    }

    m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pLightsBuffer.GetAddressOf());

    D3D11_BUFFER_DESC directLightBufferDesc = {};
    directLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    directLightBufferDesc.ByteWidth = sizeof(DirectLightConstantBuffer);
    directLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    directLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_pDevice->CreateBuffer(&directLightBufferDesc, nullptr, &m_pDirectLightBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DirectLightBuffer.");
    }

    m_pDeviceContext->PSSetConstantBuffers(2, 1, m_pDirectLightBuffer.GetAddressOf());

    D3D11_BUFFER_DESC transformBufferDesc = {};
    transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    transformBufferDesc.ByteWidth = sizeof(TransformConstantBuffer);
    transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_pDevice->CreateBuffer(&transformBufferDesc, nullptr, &m_pTransformBuffer);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create TransformBuffer.");
    }

    m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pTransformBuffer.GetAddressOf());
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
